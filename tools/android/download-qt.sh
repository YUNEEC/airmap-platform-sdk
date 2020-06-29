#!/bin/bash
set -ex

version=$1
outputDir=$1

case "${version}" in
    5.12.3)
        url=https://2c70f0fa-1d5f-fbbd-e789-ead53fd00456.s3.eu-central-1.amazonaws.com/CI/datapilot/qt_5_12_3.tar.gz
        file=qt_5_12_3.tar.gz
        sha1=c494fde2cf6d3a342d4ba731cb01464095033e75
        ;;
    *)
        echo "unknown qt version ${version}"
        exit 1
        ;;
esac

output=$outputDir/${file}

if ! [ -e ${output} ] ; then
    curl --output ${output} ${url} 2> /dev/null
fi

echo "${sha1} ${output}" | sha1sum -c - > /dev/null

dir=$(tar --exclude=*/* -tf ${output} | head -1)
tar zxf ${output} -C $outputDir
unzip -qq -o ${output} -d $outputDir
echo ${outputDir}/${dir}
