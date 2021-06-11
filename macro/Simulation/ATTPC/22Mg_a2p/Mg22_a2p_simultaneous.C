void Mg22_a2p_simultaneous(Int_t nEvents = 100, TString mcEngine = "TGeant4")
{
    // New folder
    //system("mkdir data"); // To-do check if folder already exists, if not mkdir

    // Environment directory
    TString dir = getenv("VMCWORKDIR");

    // Output file name
    TString outFile ="data/prueba.root";

    // Parameter file name
    TString parFile="data/attpcpar.root";

    // -----   Timer   --------------------------------------------------------
    TStopwatch timer;
    timer.Start();
    // ------------------------------------------------------------------------

    AtVertexPropagator* vertex_prop = new AtVertexPropagator();


    // -----   Create simulation run   ----------------------------------------
    FairRunSim* run = new FairRunSim();
    run->SetName(mcEngine);              // Transport engine
    run->SetOutputFile(outFile);          // Output file
    FairRuntimeDb* rtdb = run->GetRuntimeDb();
    // ------------------------------------------------------------------------


    // -----   Create media   -------------------------------------------------
    run->SetMaterials("media.geo");       // Materials
    // ------------------------------------------------------------------------

    // -----   Create geometry   ----------------------------------------------

    FairModule* cave= new AtCave("CAVE");
    cave->SetGeometryFileName("cave.geo");
    run->AddModule(cave);

    //FairModule* magnet = new AtMagnet("Magnet");
    //run->AddModule(magnet);

    /*FairModule* pipe = new AtPipe("Pipe");
    run->AddModule(pipe);*/

    FairDetector* ATTPC = new AtTpc("ATTPC", kTRUE);
    ATTPC->SetGeometryFileName("ATTPC_e15250_v1.root");
    //ATTPC->SetModifyGeometry(kTRUE);
    run->AddModule(ATTPC);

    // -----   Magnetic field   -------------------------------------------
    // Constant Field
    AtConstField  *fMagField = new AtConstField();
    fMagField->SetField(0., 0. ,20. ); // values are in kG
    fMagField->SetFieldRegion(-50, 50,-50, 50, -10,230); // values are in cm (xmin,xmax,ymin,ymax,zmin,zmax)
    run->SetField(fMagField);
    // --------------------------------------------------------------------

    // -----   Create PrimaryGenerator   --------------------------------------
    FairPrimaryGenerator* primGen = new FairPrimaryGenerator();

    // Beam Information
    Int_t z = 12;  // Atomic number
    Int_t a = 22;  // Mass number
    Int_t q = 0;   // Charge State
    Int_t m = 1;   // Multiplicity
    Double_t px = 0.000/a;  // X-Momentum / per nucleon!!!!!!
    Double_t py = 0.000/a;  // Y-Momentum / per nucleon!!!!!!
    Double_t pz = 2.126/a;  // Z-Momentum / per nucleon!!!!!!
    Double_t BExcEner = 0.0;
    Double_t Bmass = 21.999573843; //Mass in GeV
    Double_t NomEnergy = 120.0; //---110---  //Nominal Energy of the beam: Only used for cross section calculation (Tracking energy is determined with momentum). TODO: Change this to the energy after the IC
    // Double_t TargetMass = 4.00150618;

    AtTPCIonGenerator* ionGen = new AtTPCIonGenerator("Ion",z,a,q,m,px,py,pz,BExcEner,Bmass,NomEnergy);
    ionGen->SetSpotRadius(0,-100,0);

    primGen->AddGenerator(ionGen);

    // Variables for 2-Body kinematics reaction
    std::vector<Int_t> Zp; // Zp
    std::vector<Int_t> Ap; // Ap
    std::vector<Int_t> Qp;//Electric charge
    Int_t mult;  //Number of particles
    std::vector<Double_t> Pxp; //Px momentum X
    std::vector<Double_t> Pyp; //Py momentum Y
    std::vector<Double_t> Pzp; //Pz momentum Z
    std::vector<Double_t> Mass; // Masses
    std::vector<Double_t> ExE; // Excitation energy
    Double_t ResEner; // Energy of the beam (Useless for the moment)

    //-------------------------------------------------------------------------
    //Set the parameters of the decay generator

    std::vector<std::vector<Int_t>> zDecay;
    std::vector<std::vector<Int_t>> aDecay;
    std::vector<std::vector<Int_t>> qDecay;
    std::vector<std::vector<Double_t>> massDecay;

    Int_t zB;
    Int_t aB;
    Double_t massDecayB;
    Double_t massTarget;
    Double_t exEnergy;
    std::vector<Double_t> SepEne;

    Int_t TotDecayCases=1;//the number of decay channel (case) to be considered

    zDecay.resize(TotDecayCases);
    aDecay.resize(TotDecayCases);
    qDecay.resize(TotDecayCases);
    massDecay.resize(TotDecayCases);

    //NB: There are two ways of running the simulation
    // 1) Beam generator + decay
    //	1.a) Decay starting from the beam (decaying nucleus) + target four momentum + excitation energy -> decay products
    //    1.b) Decay from compound nucleus formed in fusion (decaying nucleus) + excitation energy -> decay products
    // 2) Beam generator + reaction generator (i.e. two-body) + decay. This case is analogous to 1.b) but the compound nucleus is the scattered particle in the       reaction that carries Ex energy. This mode has to be enabled with decay->SetSequentialDecay(kTRUE)

    // Example of 1.b)
    //--- decaying nucleus (sequential decay case) -----
    //should be a reaction product (its momentum is set in the reaction generator)
    /*zB=21; // 41Sc
    aB=41;
    massDecayB=40.96925110;
    massTarget= 0.0;
    exEnergy = 0;*/

    // Example of 1.a)
    //--- decaying nucleus (Beam + decay case or simulatenous) -----
    zB=12; //
    aB=22;
    massDecayB=21.999573843; //37K
    massTarget=4.00260325415 ; //4He //Only applicable for one-step decay (i.e.: simultaneous Beam+Decay)
    exEnergy = 0;

    //Case 1
    SepEne.push_back(5.504); //proton separation energy of mother nucleus
    zDecay.at(0).push_back(12);
    aDecay.at(0).push_back(22);
    qDecay.at(0).push_back(0);
    massDecay.at(0).push_back(21.999573843);

    zDecay.at(0).push_back(1); //proton
    aDecay.at(0).push_back(1);
    qDecay.at(0).push_back(0);
    massDecay.at(0).push_back(1.00783);

    zDecay.at(0).push_back(1); //proton
    aDecay.at(0).push_back(1);
    qDecay.at(0).push_back(0);
    massDecay.at(0).push_back(1.00783);

    AtTPCIonDecay* decay = new AtTPCIonDecay(&zDecay, &aDecay, &qDecay, &massDecay,
                                             zB, aB, massDecayB,massTarget,exEnergy,&SepEne);

    primGen->AddGenerator(decay);

    // ------------------------------------------------------------------------

    run->SetGenerator(primGen);

    // -----   Initialize simulation run   ------------------------------------
    run->Init();
    // ------------------------------------------------------------------------

    // -----   Runtime database   ---------------------------------------------

    Bool_t kParameterMerged = kTRUE;
    FairParRootFileIo* parOut = new FairParRootFileIo(kParameterMerged);
    parOut->open(parFile.Data());
    rtdb->setOutput(parOut);
    rtdb->saveOutput();
    rtdb->print();
    // ------------------------------------------------------------------------

    // -----   Start run   ----------------------------------------------------
    run->Run(nEvents);

    //You can export your ROOT geometry to a separate file
    //run->CreateGeometryFile("./data/geofile_d2He_full.root");
    // ------------------------------------------------------------------------

    // -----   Finish   -------------------------------------------------------
    timer.Stop();
    Double_t rtime = timer.RealTime();
    Double_t ctime = timer.CpuTime();
    cout << endl << endl;
    cout << "Macro finished succesfully." << endl;
    cout << "Output file is "    << outFile << endl;
    cout << "Parameter file is " << parFile << endl;
    cout << "Real time " << rtime << " s, CPU time " << ctime
         << "s" << endl << endl;
    // ------------------------------------------------------------------------



}