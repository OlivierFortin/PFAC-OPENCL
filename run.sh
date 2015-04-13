#!/bin/bash
export PATH=/usr/local/browndeer/bin:$PATH
export LD_LIBRARY_PATH=/opt/adapteva/esdk/tools/host/lib:/opt/adapteva/esdk/tools/e-gnu/lib:/usr/local/browndeer/lib:/usr/local/lib:$LD_LIBRARY_PATH
export EPIPHANY_HDF=/opt/adapteva/esdk/bsps/current/platform.hdf
export EPIPHANY_HOME=/opt/adapteva/esdk
export PATH=/usr/local/browndeer/bin:/opt/adapteva/esdk/tools/host/bin:/opt/adapteva/esdk/tools/e-gnu/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games
./bin/main
