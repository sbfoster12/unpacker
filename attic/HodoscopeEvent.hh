#ifndef HodoscopeEvent_HH 
#define HodoscopeEvent_HH

#include "DataProduct.hh"
#include "Waveform.hh"
#include "WaveformIntegral.hh"
#include "CalibratedIntegral.hh"

namespace dataProducts {

    class HodoscopeEvent : public DataProduct {

        public:
            HodoscopeEvent();

            // template<typename T> HodoscopeEvent(T *w);
            template<typename T> HodoscopeEvent(T *x, T *y);
            // HodoscopeEvent(dataProducts::WaveformIntegralCollection* xs, dataProducts::WaveformIntegralCollection* ys)
            // HodoscopeEvent(WaveformIntegralCollection* w);
            // HodoscopeEvent(CalibratedIntegralCollection* w);

            //Destructor
            ~HodoscopeEvent();

            // run/subrun
            int runNum;
            int subRunNum;

            // Event number
            int eventNum;

            // index of this waveform
            int waveformIndex;

            // properties to be summed
            double amplitude_x;
            double integral_x;
            double fullintegral_x;
            double amplitude_y;
            double integral_y;
            double fullintegral_y;

            // properties to be calculated
            double x;
            double y;
            double max_x;
            double max_y;

            double max_channel_x;
            int max_amc_x;
            int max_crate_x;
            double max_integral_x;            

            double max_channel_y;
            int max_amc_y;
            int max_crate_y;
            double max_integral_y;    

            int nx, ny;        

            bool containsNegativeIntegrals;

            TRefArray hodo_x;
            TRefArray hodo_y;

            void Show() const override;
            void DetermineEventLocation();

            ClassDefOverride(HodoscopeEvent,5)

    };

    typedef std::vector<HodoscopeEvent> HodoscopeEventCollection;

}

#endif // HodoscopeEvent_HH