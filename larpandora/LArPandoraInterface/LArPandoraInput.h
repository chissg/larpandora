/**
 *  @file   larpandora/LArPandoraInterface/LArPandoraInput.h
 *
 *  @brief  Helper functions for providing inputs to pandora
 */

#ifndef LAR_PANDORA_INPUT_H
#define LAR_PANDORA_INPUT_H 1

#include "lardata/ArtDataHelper/MVAReader.h"
namespace detinfo {
  class DetectorPropertiesData;
}

#include "larpandora/LArPandoraInterface/ILArPandora.h"
#include "larpandora/LArPandoraInterface/LArPandoraGeometry.h"
#include "larpandora/LArPandoraInterface/LArPandoraHelper.h"

#include "larpandoracontent/LArObjects/LArMCParticle.h"

namespace lar_pandora {

  /**
 *  @brief  LArPandoraInput class
 */
  class LArPandoraInput {
  public:
    /**
     *  @brief  Settings class
     */
    class Settings {
    public:
      /**
         *  @brief  Default constructor
         */
      Settings();

      const pandora::Pandora* m_pPrimaryPandora; ///<
      bool m_useHitWidths;                       ///<
      bool m_useBirksCorrection;                 ///<
      bool m_useActiveBoundingBox;               ///<
      int m_uidOffset;                           ///<
      int m_hitCounterOffset;                    ///<
      double m_dx_cm;                            ///<
      double m_int_cm;                           ///<
      double m_rad_cm;                           ///<
      double m_dEdX_mip;                         ///<
      double m_mips_max;                         ///<
      double m_mips_if_negative;                 ///<
      double m_mips_to_gev;                      ///<
      double m_recombination_factor;             ///<
    };

    /**
     *  @brief  Create the Pandora 2D hits from the ART hits
     *
     *  @param  evt art event being processed
     *  @param  settings the settings
     *  @param  driftVolumeMap the mapping from volume id to drift volume
     *  @param  hits the input list of ART hits for this event
     *  @param  idToHitMap to receive the mapping from Pandora hit ID to ART hit
     */
    static void CreatePandoraHits2D(const art::Event& evt,
                                    const Settings& settings,
                                    const LArDriftVolumeMap& driftVolumeMap,
                                    const HitVector& hitVector,
                                    IdToHitMap& idToHitMap);

    /**
     *  @brief  Create pandora LArTPCs to represent the different drift volumes in use
     *
     *  @param  settings the settings
     *  @param  driftVolumeList the drift volume list
     */
    static void CreatePandoraLArTPCs(const Settings& settings,
                                     const LArDriftVolumeList& driftVolumeList);

    /**
     *  @brief  Create pandora line gaps to cover dead regions between TPCs in a global drift volume approach
     *
     *  @param  settings the settings
     *  @param  driftVolumeList the drift volume list
     *  @param  listOfGaps the list of gaps
     */
    static void CreatePandoraDetectorGaps(const Settings& settings,
                                          const LArDriftVolumeList& driftVolumeList,
                                          const LArDetectorGapList& listOfGaps);

    /**
     *  @brief  Create pandora line gaps to cover any (continuous regions of) bad channels
     *
     *  @param  settings the settings
     *  @param  driftVolumeMap the mapping from volume id to drift volume
     */
    static void CreatePandoraReadoutGaps(const Settings& settings,
                                         const LArDriftVolumeMap& driftVolumeMap);

    /**
     *  @brief  Create the Pandora MC particles from the MC particles
     *
     *  @param  settings the settings
     *  @param  truthToParticles  mapping from MC truth to MC particles
     *  @param  particlesToTruth  mapping from MC particles to MC truth
     */
    static void CreatePandoraMCParticles(const Settings& settings,
                                         const MCTruthToMCParticles& truthToParticles,
                                         const MCParticlesToMCTruth& particlesToTruth,
                                         const RawMCParticleVector& generatorMCParticleVector);

    /**
     *  @brief Find all primary MCParticles in a given vector of MCParticles
     *
     *  @param mcParticleVector vector of all MCParticles to consider
     *  @param primaryMCParticleMap map containing primary MCParticles and bool indicating whether particle has been accounted for
     */
    static void FindPrimaryParticles(const RawMCParticleVector& mcParticleVector,
                                     std::map<const simb::MCParticle, bool>& primaryMCParticleMap);

    /**
     *  @brief Check whether an MCParticle can be found in a given map
     *
     *  @param mcParticle target MCParticle
     *  @param primaryMCParticleMap map containing primary MCParticles and bool indicating whether particle has been accounted for
     */
    static bool IsPrimaryMCParticle(const art::Ptr<simb::MCParticle>& mcParticle,
                                    std::map<const simb::MCParticle, bool>& primaryMCParticleMap);

    /**
     *  @brief  Create links between the 2D hits and Pandora MC particles
     *
     *  @param  settings the settings
     *  @param  hitMap mapping from Pandora hit addresses to ART hits
     *  @param  hitToParticleMap mapping from each ART hit to its underlying G4 track ID
     */
    static void CreatePandoraMCLinks2D(const Settings& settings,
                                       const HitMap& hitMap,
                                       const HitsToTrackIDEs& hitToParticleMap);

  private:
    typedef std::map<std::string, lar_content::MCProcess> MCProcessMap;

    /**
     *  @brief  Loop over MC trajectory points and identify start and end points within the detector
     *
     *  @param  settings the settings
     *  @param  particle the true particle
     *  @param  startT the first trajectory point in the detector
     *  @param  endT the last trajectory point in the detector
     */
    static void GetTrueStartAndEndPoints(const Settings& settings,
                                         const art::Ptr<simb::MCParticle>& particle,
                                         int& startT,
                                         int& endT);

    /**
     *  @brief  Loop over MC trajectory points and identify start and end points within a given cryostat and TPC
     *
     *  @param  cstat the cryostat
     *  @param  tpc the TPC
     *  @param  particle the true particle
     *  @param  startT the first trajectory point in the detector
     *  @param  endT the last trajectory point in the detector
     */
    static void GetTrueStartAndEndPoints(const unsigned int cstat,
                                         const unsigned int tpc,
                                         const art::Ptr<simb::MCParticle>& particle,
                                         int& startT,
                                         int& endT);

    /**
     *  @brief  Use detector and time services to get a true X offset for a given trajectory point
     *
     *  @param  evt event currently being processing by art
     *  @param  particle the true particle
     *  @param  nT the trajectory point
     */
    static float GetTrueX0(const art::Event& evt,
                           const art::Ptr<simb::MCParticle>& particle,
                           const int nT);

    /**
     *  @brief  Convert charge in ADCs to approximate MIPs
     *
     *  @param  settings the settings
     *  @param  hit_Charge the input charge
     *  @param  hit_View the input view number
     */
    static double GetMips(const detinfo::DetectorPropertiesData& detProp,
                          const Settings& settings,
                          const double hit_Charge,
                          const geo::View_t hit_View);

    /**
     *  @brief  Populate a map from MC process string to enumeration
     *
     *  @param  processMap the output map from MC process string to enumeration
     */
    static void FillMCProcessMap(MCProcessMap& processMap);
  };

} // namespace lar_pandora

#endif // #ifndef LAR_PANDORA_INPUT_H
