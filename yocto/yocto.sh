#!/bin/bash

DIR=~/bin
URL=https://storage.googleapis.com/git-repo-downloads/repo
REPO_INIT=https://source.codeaurora.org/external/imx/imx-manifest
build_dir=fsl-build-dir-1

#PACKAGES=libsdl1.2-dev xterm sed cvs subversion coreutils texi2html \
 #        docbook-utils python-pysqlite2 help2man make gcc g++ desktop-file-utils \
 #        libgl1-mesa-dev libglu1-mesa-dev mercurial autoconf automake groff curl lzop asciidoc

### Install Host packages

install_package()
{
	 sudo -E apt-get install $PACKAGES
}

### Setting Repo

init_repo()
{
	if [ -d "${DIR}" ]; then
		echo "$DIR is already present."
		read  -p "Do you want to delete it ?" yn
		
		case $yn in
			[Yy]* )
				echo "Deleting $DIR..."
				rm -rf $DIR;
				echo "Creating $DIR..."
				mkdir $DIR
				curl $URL  > $DIR/repo
				chmod a+x $DIR/repo
				export PATH=$DIR:$PATH
				break;;
			[Nn]* ) break;;
		    		* ) echo "Please answer yes or no.";;
		esac
	fi
}

clone_repo()
{
	mkdir ~/$build_dir
	cd ~/$build_dir
	repo init -u $REPO_INIT -b imx-linux-sumo -m imx-4.14.78-1.0.0_ga.xml
	repo sync -j8
}

#install_package
init_repo
clone_repo
