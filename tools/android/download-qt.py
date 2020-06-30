#! /usr/bin/python3

import xml.etree.ElementTree as ET
import wget
import py7zr
import os
import argparse

parser = argparse.ArgumentParser(description='Download Android QT Packages')
parser.add_argument('Version', type=str, default='5.12.3', help='QT Version')
parser.add_argument('Directory', type=str, default='/tmp', help='Installation directory')
args = parser.parse_args()

qt_version = args.Version
qt_version = qt_version.replace('.','')
platforms = ['android_x86', 'android_armv7']
base_url = "http://master.qt.io/online/qtsdkrepository/linux_x64/android/qt5_{}/".format(qt_version)

print('Downloading Updates.xml... ')
xml_path = wget.download("{}Updates.xml".format(base_url))
print(' ok')
tree = ET.parse(xml_path)
root = tree.getroot()

for platform in platforms:
    package_name = 'qt.qt5.{}.{}'.format(qt_version, platform)
    package = tree.find('./PackageUpdate/[Name="{}"]'.format(package_name))
    archives = package.find('./DownloadableArchives').text.split(', ')
    version = package.find("./Version").text

    for archive in archives:
        print('Downloading {} ...'.format(archive))
        url = base_url + package_name + "/" + version + archive
        path = wget.download(url)
        print(' ok')

        print('Extracting ...')    
        archive = py7zr.SevenZipFile(path, mode='r')
        archive.extractall(path=args.Directory)
        archive.close()

        print('Delete temporary file')
        os.remove(path)
        print('')
