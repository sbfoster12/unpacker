// Template fitter class built on top of the Eigen3 linear algebra library
//
// see https://gm2-docdb.fnal.gov/cgi-bin/private/ShowDocument?docid=4597
//
// Aaron Fienberg
// fienberg@uw.edu

#pragma once

#include <vector>
#include <numeric>
#include <cassert>
#include <type_traits>
#include <array>

#include <Eigen/Core>
#include <Eigen/Dense>

#include "TSpline.h"

#include "CubicSpline.hh"
#include "DataProduct.hh"

namespace fitter
{

	class TemplateFitter
	{
	public:
		// output data type
		typedef struct
		{
			std::vector<double> times;
			std::vector<double> scales;
			double pedestal;
			double chi2;
			bool converged;
		} Output;

		// construct with matrix dimensions (default to 0)
		TemplateFitter(int nPulses = 0, int nSamples = 0);

		// construct w/ template spline matrix dimensions
		TemplateFitter(const TSpline3 &primarySpline, int nPulses = 0,
					   int nSamples = 0);

		// construct with two templates
		TemplateFitter(const TSpline3 &primarySpline,
					   const TSpline3 &secondarySpline);

		void setTemplate(const TSpline3 &tSpline) { setPrimaryTemplate(tSpline); }

		void setPrimaryTemplate(const TSpline3 &tSpline)
		{
			templates_[0] = buildCubicSpline(tSpline);
		}

		void setSecondaryTemplate(const TSpline3 &tSpline)
		{
			templates_[1] = buildCubicSpline(tSpline);
		}

		void setInvertTemplatePattern(bool invert)
		{
			invertTemplatePattern_ = invert;
		}

		// get covariance_ij. don't call this before doing a fit
		// order of parameters is {t1 ... tn, s1 ... sn, pedestal}
		double getCovariance(int i, int j);

		std::vector<double> getResiduals() const
		{
			return std::vector<double>(deltas_.data(), deltas_.data() + deltas_.size());
		}

		// max number of iterations before giving up
		std::size_t getMaxIterations() const { return maxIterations_; }
		void setMaxIterations(std::size_t maxIters) { maxIterations_ = maxIters; }

		// target accuracy. When max step size is less than this,
		// the numerical minimization stops
		double getAccuracy() const { return accuracy_; }
		void setAccuracy(double newAccuracy) { accuracy_ = newAccuracy; }

		// get largest maximum valid time of the stored templates
		double getTMax() const
		{
			return std::max(templates_[0].getXMax(), templates_[1].getXMax());
		}

		bool getInvertTemplatePattern() const { return invertTemplatePattern_; }

		// fit() functions
		// n pulses is determined by timeGuesses.size()
		// these assume a contiguous fit region

		// version for single pulses that doesn't require a vector for time guess
		// errortype can be a number or a vector (uncertainty on each sample)
		template <typename sampleType, typename errorType = double>
		Output fit(const std::vector<sampleType> &trace, double timeGuess,
				   errorType error = 1.0)
		{
			// templatePattern_.resize(0);
			return fit(trace, std::vector<double>{timeGuess}, error);
		}

		// this call uses same uncertainty on all data points, defaults to 1.0

		template <typename sampleType, typename noiseType = double>
		Output fit(const std::vector<sampleType> &trace,
				   const std::vector<double> &timeGuesses,
				   noiseType noiseLevel = 1.0)
		{
			// use primary template for all pulses
			return multiFit(trace, timeGuesses,
							std::vector<bool>(timeGuesses.size(), false), noiseLevel);
		}

		// this call allows for different errors on each data point.
		// trace.size() must equal errors.size()

		// arbitrary error on each sample
		template <typename sampleType, typename noiseType>
		Output fit(const std::vector<sampleType> &trace,
				   const std::vector<double> &timeGuesses,
				   const std::vector<noiseType> &errors)
		{
			// use primary template for all pulses
			return multiFit(trace, timeGuesses,
							std::vector<bool>(timeGuesses.size(), false), errors);
		}

		// This version of the fit function passes in a pattern for
		// using multiple templates.
		template <typename sampleType, typename noiseType = double,
				  typename patternType>
		Output multiFit(const std::vector<sampleType> &trace,
						const std::vector<double> &timeGuesses,
						const std::vector<patternType> &tempPattern,
						noiseType noiseLevel = 1.0)
		{
			static_assert(std::is_arithmetic<sampleType>::value,
						  "trace must be vector of numbers!");
			static_assert(std::is_arithmetic<noiseType>::value,
						  "noise level must be a number!");
			static_assert(std::is_integral<patternType>::value,
						  "template pattern must be integer!");
			assert(timeGuesses.size() == tempPattern.size());
			assert(noiseLevel != 0);

			bool resized = false;

			if ((trace.size() != static_cast<std::size_t>(pVect_.rows())) ||
				(timeGuesses.size() != static_cast<std::size_t>(D_.rows())))
			{
				std::size_t oldSize = sampleTimes_.size();
				resizeMatrices(trace.size(), timeGuesses.size());
				if ((trace.size() > oldSize) || (wasDiscontiguous_))
				{
					std::iota(sampleTimes_.begin(), sampleTimes_.end(), 0.0);
				}
				resized = true;
			}
			else if (wasDiscontiguous_)
			{
				std::iota(sampleTimes_.begin(), sampleTimes_.end(), 0.0);
			}

			if ((resized) || (!isFlatNoise_) || (lastNoiseLevel_ != noiseLevel))
			{
				T_.bottomRows(1).fill(1.0 / noiseLevel);
				lastNoiseLevel_ = noiseLevel;
				isFlatNoise_ = true;
			}

			for (std::size_t i = 0; i < trace.size(); ++i)
			{
				pVect_(i) = trace[i] * T_.bottomRows(1)(0, i);
			}

			templatePattern_.resize(tempPattern.size());
			for (std::size_t i = 0; i < tempPattern.size(); ++i)
			{
				templatePattern_[i] = static_cast<bool>(tempPattern[i]);
			}

			wasDiscontiguous_ = false;
			return doFit(timeGuesses);
		}

		// This method uses a template pattern and supports arbitrary errors
		template <typename sampleType, typename noiseType, typename patternType>
		Output multiFit(const std::vector<sampleType> &trace,
						const std::vector<double> &timeGuesses,
						const std::vector<patternType> &tempPattern,
						const std::vector<noiseType> &errors)
		{
			static_assert(std::is_arithmetic<sampleType>::value,
						  "trace must be vector of numbers!");
			static_assert(std::is_arithmetic<noiseType>::value,
						  "errors must be vector of numbers!");
			static_assert(std::is_integral<patternType>::value,
						  "template pattern must be integer!");
			assert(errors.size() == trace.size());
			assert(timeGuesses.size() == tempPattern.size());

			if ((trace.size() != static_cast<std::size_t>(pVect_.rows())) ||
				(timeGuesses.size() != static_cast<std::size_t>(D_.rows())))
			{
				std::size_t oldSize = sampleTimes_.size();
				resizeMatrices(trace.size(), timeGuesses.size());
				if ((trace.size() > oldSize) || (wasDiscontiguous_))
				{
					std::iota(sampleTimes_.begin(), sampleTimes_.end(), 0.0);
				}
			}
			else if (wasDiscontiguous_)
			{
				std::iota(sampleTimes_.begin(), sampleTimes_.end(), 0.0);
			}

			for (std::size_t i = 0; i < trace.size(); ++i)
			{
				T_.bottomRows(1)(0, i) = 1.0 / errors[i];
				pVect_(i) = trace[i] * T_.bottomRows(1)(0, i);
			}
			isFlatNoise_ = false;

			templatePattern_.resize(tempPattern.size());
			for (std::size_t i = 0; i < tempPattern.size(); ++i)
			{
				templatePattern_[i] = static_cast<bool>(tempPattern[i]);
			}

			wasDiscontiguous_ = false;
			return doFit(timeGuesses);
		}

		// discontiguousFit() functions for fitting discontiguous regions
		// these are mainly useful for clipped pulses
		// you must pass in vector of sample times along with vector of sample values

		// single pulse version
		template <typename sampleType, typename timeType, typename errorType = double>
		Output discontiguousFit(const std::vector<sampleType> &trace,
								const std::vector<timeType> &sampleTimes,
								double timeGuess, errorType error = 1.0)
		{
			// templatePattern_.resize(0);
			return discontiguousFit(trace, sampleTimes, std::vector<double>{timeGuess},
									error);
		}

		// discontiguous fit with flat errors
		template <typename sampleType, typename timeType, typename noiseType = double>
		Output discontiguousFit(const std::vector<sampleType> &trace,
								const std::vector<timeType> &sampleTimes,
								const std::vector<double> &timeGuesses,
								noiseType noiseLevel = 1.0)
		{
			// use primary template for all pulses
			return discontiguousMultiFit(trace, sampleTimes, timeGuesses,
										 std::vector<bool>(timeGuesses.size(), false),
										 noiseLevel);
		}

		// discontiguous fit with arbitrary errors.
		template <typename sampleType, typename timeType, typename noiseType>
		Output discontiguousFit(const std::vector<sampleType> &trace,
								const std::vector<timeType> &sampleTimes,
								const std::vector<double> &timeGuesses,
								const std::vector<noiseType> &errors)
		{
			// use primary template for all pulses
			return discontiguousMultiFit(trace, sampleTimes, timeGuesses,
										 std::vector<bool>(timeGuesses.size(), false),
										 errors);
		}

		// Multi-template, discontiguous fit with flat errors
		template <typename sampleType, typename timeType, typename patternType,
				  typename noiseType = double>
		Output discontiguousMultiFit(const std::vector<sampleType> &trace,
									 const std::vector<timeType> &sampleTimes,
									 const std::vector<double> &timeGuesses,
									 const std::vector<patternType> &tempPattern,
									 noiseType noiseLevel = 1.0)
		{
			static_assert(std::is_arithmetic<sampleType>::value,
						  "trace must be vector of numbers!");
			static_assert(std::is_arithmetic<timeType>::value,
						  "sampleTimes must be vector of numbers!");
			static_assert(std::is_integral<patternType>::value,
						  "template pattern must be integer!");
			static_assert(std::is_arithmetic<noiseType>::value,
						  "errors must be vector of numbers!");
			assert(timeGuesses.size() == tempPattern.size());
			assert(trace.size() == sampleTimes.size());

			bool resized = false;

			if ((trace.size() != static_cast<std::size_t>(pVect_.rows())) ||
				(timeGuesses.size() != static_cast<std::size_t>(D_.rows())))
			{
				resizeMatrices(trace.size(), timeGuesses.size());
				resized = true;
			}

			std::copy(sampleTimes.begin(), sampleTimes.end(), sampleTimes_.begin());

			if ((resized) || (!isFlatNoise_) || (lastNoiseLevel_ != noiseLevel))
			{
				T_.bottomRows(1).fill(1.0 / noiseLevel);
				lastNoiseLevel_ = noiseLevel;
				isFlatNoise_ = true;
			}

			for (std::size_t i = 0; i < trace.size(); ++i)
			{
				pVect_(i) = trace[i] * T_.bottomRows(1)(0, i);
			}

			templatePattern_.resize(tempPattern.size());
			for (std::size_t i = 0; i < tempPattern.size(); ++i)
			{
				templatePattern_[i] = static_cast<bool>(tempPattern[i]);
			}

			wasDiscontiguous_ = true;
			return doFit(timeGuesses);
		}

		// Multi-template, discontiguous fit with arbitrary errors
		template <typename sampleType, typename timeType, typename patternType,
				  typename noiseType>
		Output discontiguousMultiFit(const std::vector<sampleType> &trace,
									 const std::vector<timeType> &sampleTimes,
									 const std::vector<double> &timeGuesses,
									 const std::vector<patternType> &tempPattern,
									 const std::vector<noiseType> &errors)
		{
			static_assert(std::is_arithmetic<sampleType>::value,
						  "trace must be vector of numbers!");
			static_assert(std::is_arithmetic<timeType>::value,
						  "sampleTimes must be vector of numbers!");
			static_assert(std::is_integral<patternType>::value,
						  "template pattern must be integer!");
			static_assert(std::is_arithmetic<noiseType>::value,
						  "errors must be vector of numbers!");
			assert(timeGuesses.size() == tempPattern.size());
			assert(trace.size() == sampleTimes.size());
			assert(errors.size() == trace.size());

			if ((trace.size() != static_cast<std::size_t>(pVect_.rows())) ||
				(timeGuesses.size() != static_cast<std::size_t>(D_.rows())))
			{
				resizeMatrices(trace.size(), timeGuesses.size());
			}

			std::copy(sampleTimes.begin(), sampleTimes.end(), sampleTimes_.begin());

			for (std::size_t i = 0; i < trace.size(); ++i)
			{
				T_.bottomRows(1)(0, i) = 1.0 / errors[i];
				pVect_(i) = trace[i] * T_.bottomRows(1)(0, i);
			}
			isFlatNoise_ = false;

			templatePattern_.resize(tempPattern.size());
			for (std::size_t i = 0; i < tempPattern.size(); ++i)
			{
				templatePattern_[i] = static_cast<bool>(tempPattern[i]);
			}

			wasDiscontiguous_ = true;
			return doFit(timeGuesses);
		}

	private:
		static CubicSpline buildCubicSpline(
			const TSpline3 &tSpline,
			CubicSpline::BoundaryType cond = CubicSpline::BoundaryType::first);

		Output doFit(const std::vector<double> &timeGuesses);

		void evalTemplates(const std::vector<double> &tGuesses);

		bool hasConverged() const;

		void calculateCovarianceMatrix();

		void resizeMatrices(int nSamples, int nPulses);

		// how small largest time step has to go before stopping minimization
		double accuracy_;
		// max number of iterations
		std::size_t maxIterations_;
		// whether covariance matrix is ready
		bool covReady_;

		double lastNoiseLevel_;
		bool isFlatNoise_;
		bool wasDiscontiguous_;

		// templates
		// templates_[0] is primary template, templates_[1] is secondary
		std::array<CubicSpline, 2> templates_;

		// vector of time values corresponding to each sample
		std::vector<double> sampleTimes_;
		std::vector<bool> templatePattern_;

		// eigen matrices, all kept around to avoid repeated allocation

		// vector of pulse heights weighted by inverse noise at each sample
		Eigen::VectorXd pVect_;
		// T matrix (see document)
		Eigen::MatrixXd T_;
		// linear parameters will be stored in here after each fit
		Eigen::VectorXd linearParams_;
		// vector of pulse minus fit function over noise
		Eigen::VectorXd deltas_;
		// template derivatives matrix
		Eigen::MatrixXd D_;
		// template second derivatives matrix
		Eigen::MatrixXd D2_;
		// hessian
		Eigen::MatrixXd Hess_;
		// covariance matrix
		Eigen::MatrixXd Cov_;
		// proposed time steps to minimum
		Eigen::VectorXd timeSteps_;

		// whether to invert the template pattern
		// allows the beam fitter and laser fitter to be stored
		// in a single object
		//
		// Aaron Fienberg
		// May 2019
		bool invertTemplatePattern_;
	};
}
