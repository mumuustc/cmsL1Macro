
// system include files
#include <memory>
#include <iostream>
#include <fstream>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "CalibFormats/HcalObjects/interface/HcalDbService.h"
#include "CalibFormats/HcalObjects/interface/HcalDbRecord.h"
#include "CalibFormats/HcalObjects/interface/HcalCoderDb.h"
#include "CalibFormats/HcalObjects/interface/HcalCalibrations.h"
#include "CondFormats/HcalObjects/interface/HcalQIEShape.h"

#include "DataFormats/HcalDetId/interface/HcalDetId.h"
#include "DataFormats/HcalDetId/interface/HcalElectronicsId.h"
#include "DataFormats/HcalDigi/interface/HcalQIESample.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "DataFormats/HcalDetId/interface/HcalSubdetector.h"

#include "Geometry/HcalCommonData/interface/HcalDDDRecConstants.h"
#include "Geometry/CaloTopology/interface/HcalTopology.h"

#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "TTree.h"

// class declaration
//

class HFAdcToGeV : public edm::EDAnalyzer {
public:
  explicit HFAdcToGeV(const edm::ParameterSet&);
  ~HFAdcToGeV();
  
  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

  edm::EDGetTokenT<QIE10DigiCollection> tok_hfQIE10_;

  int nampl_;
  std::vector<int> ieta_;
  std::vector<int> depth_;
  std::vector<int> iphi_;
  std::vector<int> subdet_;

  std::vector<double> charge_;
  std::vector<double> charge_ped_;
  std::vector<double> energy_;
  std::vector<double> energy_ped_;

  std::vector<int> ampl_;

private:
  // L1GtUtils m_l1GtUtils;
  edm::Service<TFileService> fs;
  TTree *root;
  
  virtual void beginJob() override;
  virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
  virtual void endJob() override;
  
  virtual void beginRun(edm::Run const&, edm::EventSetup const&) override;
  //virtual void endRun(edm::Run const&, edm::EventSetup const&) override;
  //virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
  //virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;

  // ----------member data ---------------------------

  edm::ESHandle<HcalDbService> conditions;
  const HcalDDDRecConstants *hcons;
  const HcalTopology *htopology;
  const int nchannel = 3456;
  int maxDepth_[5]; // 0:any, 1:HB, 2:HE, 3:HO, 4:HF

  std::string color_red = "\033[31;1m";
  std::string color_yellow = "\033[33;1m";
  std::string color_nc = "\033[0m";

};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
HFAdcToGeV::HFAdcToGeV(const edm::ParameterSet& iConfig)
// m_l1GtUtils(iConfig, consumesCollector(), true)//this is important for 80x to compile
{
  const edm::InputTag hcalDigis("hcalDigis");
  tok_hfQIE10_ = consumes<QIE10DigiCollection>(iConfig.getUntrackedParameter<edm::InputTag>("digiLabel", hcalDigis));
  // tok_hfQIE10_ = consumes<QIE10DigiCollection>(inputLabel_);
  /* inputLabel_(conf.getParameter<edm::InputTag>("digiLabel")) */
}


HFAdcToGeV::~HFAdcToGeV()
{
 
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)

}

//
// member functions
//

// ------------ method called for each event  ------------
void HFAdcToGeV::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  // using namespace edm;
  
  ieta_.clear();
  iphi_.clear();
  depth_.clear();
  subdet_.clear();

  charge_.clear();
  charge_ped_.clear();
  energy_.clear();
  energy_ped_.clear();

  ampl_.clear();  

  iSetup.get<HcalDbRecord>().get(conditions);
  edm::Handle<QIE10DigiCollection> digi;
  bool getdigitag = iEvent.getByToken(tok_hfQIE10_, digi);
  if(!getdigitag) std::cout << color_red+"HFAdcToGeV::analyze : invalid digiLabel ( - tok_hfQIE10_)"+color_nc << std::endl; 
  // else std::cout << color_yellow+"HFAdcToGeV::analyze : good digiLabel ( - tok_hfQIE10_)"+color_nc << std::endl;
  CaloSamples tool;

  const unsigned inputSize = digi->size();
  // std::cout << color_yellow+"HFAdcToGeV::analyze : digi->size() = " << inputSize << color_nc <<std::endl;
  nampl_ = inputSize;
  if ( inputSize )
    {
      for ( auto& it : *digi ) // QIE10DigiCollection::const_iterator
        {
          const QIE10DataFrame& frame(it);
          const HcalDetId cell(frame.id());
          int ieta = cell.ieta();
          int depth = cell.depth();
          int iphi = cell.iphi();
          int sub = cell.subdet();

          if(sub != HcalSubdetector::HcalForward) continue; // DataFormats/HcalDetId/interface/HcalSubdetector.h
          if(depth > maxDepth_[sub])
            {
              edm::LogWarning("HcalDetId") << color_red+"HcalDetID presents conflicting information. "
                                           << "Depth: " << depth << ", iphi: " << iphi << ", ieta: " << ieta 
                                           << ". Max depth from geometry is: " << maxDepth_[sub] << color_nc;
              continue;
            }

          HcalCalibrations calibrations = conditions->getHcalCalibrations(cell);
          const HcalQIECoder* channelCoder = conditions->getHcalCoder(cell);
          const HcalQIEShape* shape = conditions->getHcalShape(channelCoder);
          HcalCoderDb coder(*channelCoder, *shape);
          coder.adc2fC(frame, tool);
 
          if(frame.samples() != tool.size()) { edm::LogError("HFAdcToGeV") << "frame.samples() != tool.size()"; break; }

          int soi = tool.presamples();
          // int lastbin = tool.size() - 1;
          double charge = 0.;
          double charge_ped = 0;
          double energy = 0;
          double energy_ped = 0;
          int ampl = 0;
          for (int ii = 0; ii < tool.size(); ii++) 
            {
              QIE10DataFrame::Sample sam = frame[ii];
              if( (sam.soi() && ii != soi) || (!sam.soi() && ii == soi) ) { edm::LogError("HFAdcToGeV") << "Wrong soi information"; break; }
              int capid = sam.capid();
              int adc = sam.adc();
              if( ii != soi) continue;
              // if( ii < soi || ii > lastbin ) continue;
              charge += (tool[ii] - calibrations.pedestal(capid));
              energy += charge * calibrations.respcorrgain(capid);
              charge_ped += tool[ii];
              energy_ped += charge_ped * calibrations.respcorrgain(capid);
              ampl += adc;
            }

          ieta_.push_back(ieta);
          iphi_.push_back(iphi);
          depth_.push_back(depth);
          subdet_.push_back(sub);

          charge_.push_back(charge);
          charge_ped_.push_back(charge_ped);
          energy_.push_back(energy);
          energy_ped_.push_back(energy_ped);

          ampl_.push_back(ampl);
        }
    }
  root->Fill();
}

// ------------ method called once each job just before starting event loop  ------------

void 
HFAdcToGeV::beginJob()
{
  root = fs->make<TTree>("adc","adc");
  root->Branch("nampl", &nampl_, "nampl/I");
  root->Branch("ieta", &ieta_);
  root->Branch("iphi", &iphi_);
  root->Branch("depth", &depth_);

  root->Branch("charge", &charge_);
  root->Branch("charge_ped", &charge_ped_);
  root->Branch("energy", &energy_);
  root->Branch("energy_ped", &energy_ped_);

  root->Branch("ampl", &ampl_);
}

// ------------ method called once each job just after ending the event loop  ------------
void 
HFAdcToGeV::endJob() 
{
}

// ------------ method called when starting to processes a run  ------------

void 
HFAdcToGeV::beginRun(edm::Run const& iRun, edm::EventSetup const& iSetup)
{
  /*
    bool useL1EventSetup = true;
    bool useL1GtTriggerMenuLite = false;
  
    // could be added in beginRun(...) - if not added, the caching will be done in analyze/produce/filter method
    m_l1GtUtils.getL1GtRunCache(iRun, iSetup, useL1EventSetup, useL1GtTriggerMenuLite);
  */
  edm::ESHandle<HcalDDDRecConstants> pHRNDC;
  iSetup.get<HcalRecNumberingRecord>().get( pHRNDC );
  hcons = &(*pHRNDC);
   
  htopology = new HcalTopology(hcons);
  maxDepth_[1] = hcons->getMaxDepth(0); // HB
  maxDepth_[2] = hcons->getMaxDepth(1); // HE
  maxDepth_[3] = hcons->getMaxDepth(3); // HO
  maxDepth_[4] = hcons->getMaxDepth(2); // HF
  maxDepth_[0] = (maxDepth_[1] > maxDepth_[2] ? maxDepth_[1] : maxDepth_[2]);
  maxDepth_[0] = (maxDepth_[0] > maxDepth_[3] ? maxDepth_[0] : maxDepth_[3]);
  maxDepth_[0] = (maxDepth_[0] > maxDepth_[4] ? maxDepth_[0] : maxDepth_[4]); // any of HB/HE/HO/HF

}

// ------------ method called when ending the processing of a run  ------------
/*
  void 
  HFAdcToGeV::endRun(edm::Run const&, edm::EventSetup const&)
  {
  }
*/

// ------------ method called when starting to processes a luminosity block  ------------
/*
  void 
  HFAdcToGeV::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
  {
  }
*/

// ------------ method called when ending the processing of a luminosity block  ------------
/*
  void 
  HFAdcToGeV::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
  {
  }
*/

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
HFAdcToGeV::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(HFAdcToGeV);
