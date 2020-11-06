# voxblox

Original repository: https://github.com/ethz-asl/voxblox


## Build
```bash
cd voxblox/docker
./build.sh
```

## Run

### On host:
```bash
roscore
```

### On docker image:
```bash
cd voxblox/docker
./run.sh -v data.bag:/tmp/data.bag

roslaunch voxblox_ros cow_and_lady_dataset.launch bag_file:=/tmp/data.bag
```

```bash
# another docker image
cd voxblox/docker
./run.sh

rviz -d docker/voxblox.rviz
```

data.bag (cow and lady dataset): https://projects.asl.ethz.ch/datasets/doku.php?id=iros2017/

![voxblox](https://user-images.githubusercontent.com/31344317/98347894-61cefd00-205b-11eb-97d7-45dcb501014e.gif)
