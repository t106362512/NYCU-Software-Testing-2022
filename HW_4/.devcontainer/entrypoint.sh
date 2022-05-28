#!/usr/bin/env bash

wget https://repo1.maven.org/maven2/junit/junit/4.12/junit-4.12.jar
wget https://repo1.maven.org/maven2/org/hamcrest/hamcrest-core/1.3/hamcrest-core-1.3.jar
wget https://cs.gmu.edu/~offutt/mujava/{mujava.jar,openjava.jar,mujava.config}

export CLASSPATH=${PWD}/*:$CLASSPATH
echo "MuJava_HOME=${PWD}" > mujava.config

exec "$@"

xvfb-run java mujava.gui.GenMutantsMain
xvfb-run java mujava.gui.RunTestMain