

#include "svol_comparison.h" // function header

#include "svol_bs.h" // bs filter for svol
#include "svol_apf.h" // apf filter for svol
#include "svol_sisr.h" // sisr filter for svol

#include "utils.h"   // for readInData()
#include "resamplers.h" // for mn_resampler

// some template parameters
#define dimstate 1
#define dimobs   1
#define numparts 50000

void run_svol_comparison(const std::string &csv)
{
        
    // make some models
    double phi = .91;
    double beta = .5;
    double sigma = 1.0;
    svol_bs<numparts, dimstate, dimobs,mn_resampler<numparts,dimstate>> bssvol(phi, beta, sigma);
    svol_apf<numparts,dimstate,dimobs,mn_resampler<numparts,dimstate> > apfsvol(phi,beta,sigma);
    svol_sisr<numparts,dimstate,dimobs,mn_resampler<numparts,dimstate> > sisrsvol(phi,beta,sigma);

    // read in some data
    std::vector<Eigen::Matrix<double,dimobs,1>> data = utils::readInData<dimobs>(csv);

    // iterate over the data
    for(size_t row = 0; row < data.size(); ++row){
        bssvol.filter(data[row]);
        apfsvol.filter(data[row]);
        sisrsvol.filter(data[row]);
        
        std::cout << bssvol.getLogCondLike() << ", " << apfsvol.getLogCondLike() <<", " << sisrsvol.getLogCondLike() << "\n";
    }
    
}