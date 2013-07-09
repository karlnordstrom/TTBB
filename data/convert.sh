#! /bin/bash

for name in $(ls EVNT*); do
	Reco_trf.py preExec="rec.doApplyAODFix.set_Value_and_Lock(False);from D3PDMakerConfig.D3PDMakerFlags import D3PDMakerFlags;D3PDMakerFlags.TruthWriteHadrons=True" inputEVNTFile=/afs/phas.gla.ac.uk/user/k/knordstrom/data/truth/$name outputNTUP_TRUTHFile=/afs/phas.gla.ac.uk/user/k/knordstrom/data/truth/ntup/$name
done

exit
