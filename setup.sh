if [[ ! -d $EMSDK ]]; then
  echo "Be sure to set the EMSDK environment variable."
  exit 1
fi

$EMSDK/emsdk activate latest &&\
source $EMSDK/emsdk_env.sh &&\
git clone 'https://chromium.googlesource.com/chromium/tools/depot_tools.git' &&\
export PATH="${PWD}/depot_tools:${PATH}" &&\
git submodule update --init --recursive &&\
$EMSDK/emsdk activate latest &&\
source $EMSDK/emsdk_env.sh &&\
cd third_party/Skia &&\
python2 tools/git-sync-deps