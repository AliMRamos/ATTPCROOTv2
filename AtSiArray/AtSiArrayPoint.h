#ifndef AtSIARRAYPOINT_H
#define AtSIARRAYPOINT_H 1

#include "FairMCPoint.h"

#include "TObject.h"
#include "TVector3.h"

class AtSiArrayPoint : public FairMCPoint {

public:
   /** Default constructor **/
   AtSiArrayPoint();

   /** Constructor with arguments
    *@param trackID  Index of MCTrack
    *@param detID    Detector ID
    *@param pos      Ccoordinates at entrance to active volume [cm]
    *@param mom      Momentum of track at entrance [GeV]
    *@param tof      Time since event start [ns]
    *@param length   Track length since creation [cm]
    *@param eLoss    Energy deposit [GeV]
    **/
   AtSiArrayPoint(Int_t trackID, Int_t detID, TVector3 pos, TVector3 mom, Double_t tof, Double_t length,
                  Double_t eLoss);

   AtSiArrayPoint(Int_t trackID, Int_t detID, TString VolName, Int_t detCopyID, TVector3 posIn, TVector3 posOut,
                  TVector3 momIn, TVector3 momOut, Double_t tof, Double_t length, Double_t eLoss, Double_t EIni,
                  Double_t AIni, Int_t A, Int_t Z);

   /** Destructor **/
   virtual ~AtSiArrayPoint();

   /** Accessors **/
   Int_t GetDetCopyID() const { return fDetCopyID; }
   Double_t GetXIn() const { return fX; }
   Double_t GetYIn() const { return fY; }
   Double_t GetZIn() const { return fZ; }
   Double_t GetXOut() const { return fX_out; }
   Double_t GetYOut() const { return fY_out; }
   Double_t GetZOut() const { return fZ_out; }
   Double_t GetPxOut() const { return fPx_out; }
   Double_t GetPyOut() const { return fPy_out; }
   Double_t GetPzOut() const { return fPz_out; }
   TString GetVolName() const { return fVolName; }
   Double_t GetEIni() const { return fEnergyIni; }
   Double_t GetAIni() const { return fAngleIni; }
   Int_t GetMassNum() const { return fAiso; }
   Int_t GetAtomicNum() const { return fZiso; }

   void PositionIn(TVector3 &pos) { pos.SetXYZ(fX, fY, fZ); }
   void PositionOut(TVector3 &pos) { pos.SetXYZ(fX_out, fY_out, fZ_out); }
   void MomentumOut(TVector3 &mom) { mom.SetXYZ(fPx_out, fPy_out, fPz_out); }

   /** Point coordinates at given z from linear extrapolation **/
   Double_t GetX(Double_t z) const;
   Double_t GetY(Double_t z) const;

   /** Modifiers **/
   void SetPositionOut(TVector3 pos);
   void SetMomentumOut(TVector3 mom);
   void SetDetCopyID(Int_t id) { fDetCopyID = id; };

   /** Output to screen **/
   virtual void Print(const Option_t *opt) const;

private:
   /** Copy constructor **/
   AtSiArrayPoint(const AtSiArrayPoint &point);
   AtSiArrayPoint operator=(const AtSiArrayPoint &point);

protected:
   Double32_t fX_out, fY_out, fZ_out;
   Double32_t fPx_out, fPy_out, fPz_out;
   Int_t fDetCopyID;
   TString fVolName;
   Double_t fEnergyIni;
   Double_t fAngleIni;
   Int_t fAiso;
   Int_t fZiso;

   ClassDef(AtSiArrayPoint, 1)
};

inline void AtSiArrayPoint::SetPositionOut(TVector3 pos)
{
   fX_out = pos.X();
   fY_out = pos.Y();
   fZ_out = pos.Z();
}

inline void AtSiArrayPoint::SetMomentumOut(TVector3 mom)
{
   fPx_out = mom.Px();
   fPy_out = mom.Py();
   fPz_out = mom.Pz();
}

#endif
