#include "TF1.h"
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TGraph2D.h"
#include "TGraph.h"
#include "TF1.h"

double func(double *x, double *p, TGraph2D* gr, int npeaks=1)
{
    double output = p[0];
    for (int i = 0; i < npeaks; i++)
    {
        output += p[i*2+1] * gr->Interpolate(x[0]-p[i*2+2],p[i*2+1]);
    }
    return output;
    // return p[0] * gr->Interpolate(x[0]-p[1],p[0]) + p[2];
}

TGraph* get_residuals(TGraph *gr, TF1 *func, bool mask_peaks = true, int peak_region = 20){
    TGraph* g2 = new TGraph(gr->GetN());

    double x,y;
    for (int i = 0; i < gr->GetN(); i++ )
    {
        gr->GetPoint(i,x,y);
        g2->SetPoint(i,x, y-func->Eval(x));
    }

    if(mask_peaks)
    {
        for (int i = 0; i < (func->GetNpar()-1)/2; i++)
        {
            int peak = int(func->GetParameter(i*2+2));
            std::cout << "Masking Peak: " << peak  << " +/- " << peak_region <<std::endl;
            for (int j = peak-peak_region; j < peak+peak_region; j++)
            {
                // std::cout << "    -> " << j << " / " << x << " / " << y << std::endl;
                gr->GetPoint(j,x,y);
                g2->SetPoint(j,x,0.);
            }
            // gr->GetPoint(i,x,y);
            // for (int j = int(x)-peak_region)
        }
    }

    return g2;
}

std::pair<int,double> get_maximum_index_and_value(TGraph* gr)
{
    auto y = gr->GetY();
    auto max_i = std::max_element(y, y+gr->GetN()) - y;
    return std::pair<int,double>(max_i, y[max_i]);
}

int test_cpp_implementation()
{
    TFile *f1 = new TFile("./graph.root", "OPEN");
    TGraph2D *gr = (TGraph2D*) f1->Get("Graph2D");
    // TGraph *gr2 = (TGraph*) f1->Get("trace_1");
    // TGraph *gr2 = (TGraph*) f1->Get("trace_240");
    TGraph *gr2 = (TGraph*) f1->Get("trace_9");
    // TGraph *gr2 = (TGraph*) f1->Get("trace_68");


    // TCanvas *c  = new TCanvas();
    // gr->Draw("surf1");
    // c->Draw();

    int break_counter = 0;
    // std::cout << "break counter " << break_counter << std::endl; break_counter++;
    int npar;
    TF1 *fitfunc;
    TGraph *residuals;
    std::vector<double> prev_params;
    TCanvas *c2  = new TCanvas();
    c2->Divide(2);
    c2->cd(1);
    gr2->Draw();
    std::pair<int,double> guess;
    double prev_chi2 = 1e12;

    double min_amplitude = 30.0;
    for (int n = 1; n < 6; n++)
    {
        // std::cout << "break counter " << break_counter << std::endl; break_counter++;
        npar = n*2+1;
        // continue;
        auto func2 = [gr, n](double *x, double *y) {return func(x,y,gr,n); };
        fitfunc = new TF1("fitfunc", func2, -1000,1000,npar);
        // TF1 *base_func = new TF1("base_func", func2, -1000,1000,3);
        // TF1 *fitfunc = new TF1("fitfunc", "base_func(0) + base_func(3)", -1000,1000,6);
        // std::cout << "break counter " << break_counter << std::endl; break_counter++;
        fitfunc->SetParameter(0,-1800);
        fitfunc->SetParLimits(0,-2000,2000);
        if(n == 1)
        {
            // find guess from the original grasph
            guess = get_maximum_index_and_value(gr2);
            guess.second = guess.second - gr2->GetPointY(0);
        }
        else{
            // find guess from the residuals
            guess = get_maximum_index_and_value(residuals);
        }
        std::cout << "Looking for peak " << n << " at " << guess.first <<  " / " << guess.second << std::endl;
        for(int i = 0; i < n; i++)
        {
            fitfunc->SetParameter(i*2 + 1,guess.second);
            fitfunc->SetParLimits(i*2 + 1,min_amplitude,2000);
            fitfunc->SetParameter(i*2 + 2,guess.first);
            fitfunc->SetParLimits(i*2 + 2,0,1000);
        }
        for(int i = 0; i < prev_params.size(); i++)
        {
            fitfunc->FixParameter(i,prev_params[i]);
        }
        fitfunc->SetLineColor(n+1);
        fitfunc->SetNpx(1000);

        // std::cout << "break counter " << break_counter << std::endl; break_counter++;
        // gr2->Fit(base_func, "R+");
        // fit first with parameters fixed to original values
        gr2->Fit(fitfunc, "LRQ");

        // release parameters and let everything float
        for(int i = 0; i < prev_params.size(); i++)
        {
            fitfunc->ReleaseParameter(i);
        }
        fitfunc->SetParLimits(0,-2000,2000);
        for(int i = 0; i < n; i++)
        {
            fitfunc->SetParLimits(i*2 + 1,min_amplitude,2000);
            fitfunc->SetParLimits(i*2 + 2,0,1000);
        }
        gr2->Fit(fitfunc, "LR+");
        if(fitfunc->GetChisquare()/fitfunc->GetNDF() < prev_chi2)
        {
            fitfunc->DrawCopy("same");

            residuals = get_residuals(gr2, fitfunc);
            residuals->SetLineColor(n+1);
            c2->cd(2);
            if(n > 1)
            {
                residuals->DrawClone("same");
            }
            else
            {
                residuals->DrawClone();
            }
            c2->cd(1);

            // std::cout << "break counter " << break_counter << std::endl; break_counter++;
            std::cout << "Fit was made better, trying another..." << std::endl;
            prev_params.clear();
            for (int i = 0; i< npar; i++)
            {
                prev_params.push_back(fitfunc->GetParameter(i));
            }
            prev_chi2 = fitfunc->GetChisquare()/fitfunc->GetNDF();
        }
        else{
            std::cout << "Fit was not made better" << std::endl;
            break;
        }
    }
    std::cout << "Final fit has " << (prev_params.size()-1)/2 << " peaks and parameters:\n";
    for (int i = 0; i < prev_params.size(); i++)
    {
        std::cout << "    -> " << i << " = " << prev_params.at(i) << "\n";
    }
    std::cout << std::endl;

    
    // fitfunc->Draw("same");
    c2->Draw();




    return 0;
}