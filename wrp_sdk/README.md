# Sample Code for wrp_sdk

## Install the SDK

If you're using Ubuntu 18.04:

```
$ echo "deb https://westonrobot.jfrog.io/artifactory/wrtoolbox-release bionic main" | sudo tee /etc/apt/sources.list.d/weston-robot.list
```

If you're using Ubuntu 20.04:

```
$ echo "deb https://westonrobot.jfrog.io/artifactory/wrtoolbox-release focal main" | sudo tee /etc/apt/sources.list.d/weston-robot.list
```

Update the package manager and install wrp_sdk:

```
$ curl -sSL 'https://westonrobot.jfrog.io/artifactory/api/security/keypair/wr-deb/public' | sudo apt-key add -
$ sudo apt-get update
$ sudo apt-get install wrp_sdk
```

## Build the sample code

```
$ mkdir build && cd build
$ cmake ..
$ make -j8
```

You will get executable "sdk_demo" inside the build folder.