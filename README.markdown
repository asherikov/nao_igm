# Inverse geometrical model for Nao.


## Requirements

* Eigen  = v. 2
* cmake >= v. 2.6
* GNU make



## Overview

The features of the currently used version of IGM are:

* One of the feet is chosen as a reference, hence there are
two IGM functions for the left and right foot respectively.
These functions are generated using Maple.

* The selection of reference foot depends on the calling module.
(Currently it is the support foot in a single support phase, and 
the last support foot in a double support phase.)

* Only the joint angles of the lower body part are controlled, 
others remain constant.

* The following constraints are imposed:
    * 3 on the position of the swing foot
    * 3 on the orientation of the swing foot
    * 3 on the position of the CoM
    * 1 to take into account the coupled joint L_HIP_YAW_PITCH / R_HIP_YAW_PITCH

* To support a steady walk, the deviation from given reference 
angles is penalized.


Note, that there are other versions of igm() function, the respective 
source files are kept in a separate branch: 

>   <https://github.com/asherikov/nao_igm/tree/MapleGeneratedFunctions>

The features of different versions are described in file 

>   ./maple_sources/Readme_M.txt



## Compilation

The following command compiles the library:

>   "make cmake"

Toolchain can be specified in the folowing way:

>   "make cmake TOOLCHAIN=< path to a toolchain >"
