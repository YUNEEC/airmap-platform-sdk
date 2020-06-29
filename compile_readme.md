# Airmap Platform SDK for Android with QT support

## Setup & Dependencies on Ubuntu

### Devtools installation

Run the following commands from the top-level Airmap Platform SDK folder to install required development packages. _NOTE: This requires root privilege_

```
$ tools/ubuntu/setup.dev.sh
```

### Android NDK r19c download and installation

Download [Android NDK r19c](https://dl.google.com/android/repository/android-ndk-r19c-linux-x86_64.zip) and unzip it to _/opt_.

### Qt5.12.3 download and installation

Download [Qt5.12.3](https://2c70f0fa-1d5f-fbbd-e789-ead53fd00456.s3.eu-central-1.amazonaws.com/CI/datapilot/qt_5_12_3.tar.gz) and extract it to _/opt_

Or download [qtcreator5.12.3](http://download.qt.io/archive/qt/5.12/5.12.3/) and install for full qtcreator enviornment.

## Build manually

### Update submodule

```
git submodule update --init --recursive
```

### Android ABI armeabi-v7a

```
$ mkdir android_armv7
$ cd android_armv7
$ cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/airmapd/android/Qt.5.12/armv7 -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON \
-DANDROID_ABI=armeabi-v7a \
-DANDROID_PLATFORM=android-16 \
-DANDROID_STL=c++_static \
-DANDROID_CPP_FEATURES="rtti exceptions" \
-DCMAK_TOOLCHAIN_FILE=/opt/android-ndk-r19c/build/cmake/android.toolchain.cmake \
-DAIRMAP_ENABLE_QT=ON \
-DQt5_DIR=/opt/5.12.3/android_armv7/lib/cmake/Qt5 \
-DQt5Core_DIR=/opt/5.12.3/android_armv7/lib/cmake/Qt5Core \
..
$ make
$ make install
```

### Android ABI x86

```
$ mkdir android_x86
$ cd android_x86
$ cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/airmapd/android/Qt.5.12/x86 -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON \
-DANDROID_ABI=x86 \
-DANDROID_PLATFORM=android-16 \
-DANDROID_STL=c++_static \
-DANDROID_CPP_FEATURES="rtti exceptions" \
-DCMAK_TOOLCHAIN_FILE=/opt/android-ndk-r19c/build/cmake/android.toolchain.cmake \
-DAIRMAP_ENABLE_QT=ON \
-DQt5_DIR=/opt/5.12.3/android_x86/lib/cmake/Qt5 \
-DQt5Core_DIR=/opt/5.12.3/android_x86/lib/cmake/Qt5Core \
..
$ make
$ make install
```
