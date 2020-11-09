from __future__ import print_function
import torch.utils.data as data
import os
import os.path
import torch
import numpy as np
import sys
from tqdm import tqdm 
import json
#from plyfile import PlyData, PlyElement

def get_segmentation_classes(root):
    catfile = os.path.join(root, 'synsetoffset2category.txt')
    cat = {}
    meta = {}

    with open(catfile, 'r') as f:
        for line in f:
            ls = line.strip().split()
            cat[ls[0]] = ls[1]

    for item in cat:
        dir_seg = os.path.join(root, cat[item], 'points_label')
        dir_point = os.path.join(root, cat[item], 'points')
        fns = sorted(os.listdir(dir_point))
        meta[item] = []
        for fn in fns:
            token = (os.path.splitext(os.path.basename(fn))[0])
            meta[item].append((os.path.join(dir_point, token + '.pts'), os.path.join(dir_seg, token + '.seg')))
    
    with open(os.path.join(os.path.dirname(os.path.realpath(__file__)), '../misc/num_seg_classes.txt'), 'w') as f:
        for item in cat:
            datapath = []
            num_seg_classes = 0
            for fn in meta[item]:
                datapath.append((item, fn[0], fn[1]))

            for i in tqdm(range(len(datapath))):
                l = len(np.unique(np.loadtxt(datapath[i][-1]).astype(np.uint8)))
                if l > num_seg_classes:
                    num_seg_classes = l

            print("category {} num segmentation classes {}".format(item, num_seg_classes))
            f.write("{}\t{}\n".format(item, num_seg_classes))

def gen_modelnet_id(root):
    classes = []
    with open(os.path.join(root, 'train.txt'), 'r') as f:
        for line in f:
            classes.append(line.strip().split('/')[0])
    classes = np.unique(classes)
    with open(os.path.join(os.path.dirname(os.path.realpath(__file__)), '../misc/modelnet_id.txt'), 'w') as f:
        for i in range(len(classes)):
            f.write('{}\t{}\n'.format(classes[i], i))

class normalDataset(data.Dataset):
    def __init__(self,
                 root,
                 shape_list_filename,
                 npoints=2048,
                 data_augmentation=True):
        self.npoints = npoints
        self.root = root
        self.shape_list_filename = shape_list_filename
        #self.catfile = os.path.join(self.root, 'synsetoffset2category.txt')
        #self.cat = {}
        self.data_augmentation = data_augmentation
        #self.classification = classification
        #self.seg_classes = {}
        #self.split=split

        # get all shape names in the dataset
        self.shape_names = []
        with open(os.path.join(root, self.shape_list_filename)) as f:
            self.shape_names = f.readlines()
        self.shape_names = [x.strip() for x in self.shape_names]
        self.shape_names = list(filter(None, self.shape_names))

        #with open(self.catfile, 'r') as f:
        #    for line in f:
        #        ls = line.strip().split()
        #        self.cat[ls[0]] = ls[1]
        #print(self.cat)
        #if not class_choice is None:
        #    self.cat = {k: v for k, v in self.cat.items() if k in class_choice}

        #self.id2cat = {v: k for k, v in self.cat.items()}

        #self.meta = {}
        #splitfile = os.path.join(self.root, 'train_test_split', 'shuffled_{}_file_list.json'.format(split))
        #from IPython import embed; embed()
        #filelist = json.load(open(splitfile, 'r'))
        #for item in self.cat:
        #    self.meta[item] = []

        #for file in filelist:
        #    _, category, uuid = file.split('/')
        #    if category in self.cat.values():
        #        self.meta[self.id2cat[category]].append((os.path.join(self.root, category, 'points', uuid+'.pts'),
        #                                os.path.join(self.root, category, 'points_label', uuid+'.seg')))

        #self.datapath = []
        #for item in self.cat:
        #    for fn in self.meta[item]:
        #        self.datapath.append((item, fn[0], fn[1]))

        #self.classes = dict(zip(sorted(self.cat), range(len(self.cat))))
        #print(self.classes)
        #with open(os.path.join(os.path.dirname(os.path.realpath(__file__)), '../misc/num_seg_classes.txt'), 'r') as f:
        #    for line in f:
        #        ls = line.strip().split()
        #        self.seg_classes[ls[0]] = int(ls[1])
        #self.num_seg_classes = self.seg_classes[list(self.cat.keys())[0]]
        #print(self.seg_classes, self.num_seg_classes)

    def __getitem__(self, index):
        #fn = self.datapath[index]
        #cls = self.classes[self.datapath[index][0]]
        point_filename = os.path.join(self.root, 'point_sets', self.shape_names[index] + '.xyz')
        query_filename = os.path.join(self.root, 'query_point_sets', self.shape_names[index] + '.xyz')
        direct_filename = os.path.join(self.root, 'query_point_direction', self.shape_names[index] + '.direct')
        normal_filename= os.path.join(self.root, 'normals', self.shape_names[index] + '.normals')
        mmap_mode = None
        # read whole point sets
        pts_np = np.load(point_filename + '.npy', mmap_mode=mmap_mode)
        if pts_np.shape[1] > 3:
            pts_np = pts_np[:, 0:3]
        if pts_np.dtype != np.float32:
            pts_np = pts_np.astype(np.float32)
        # read query point sets
        query_pts_np = np.load(query_filename + '.npy', mmap_mode=mmap_mode)
        if query_pts_np.shape[1] > 3:
            query_pts_np = query_pts_np[:, 0:3]
        if query_pts_np.dtype != np.float32:
            query_pts_np = query_pts_np.astype(np.float32)
        #read query points direction and normals
        direction_np = np.load(direct_filename + '.npy', mmap_mode=mmap_mode)
        if direction_np.dtype != np.float32:
            direction_np = direction_np.astype(np.float32)
        normals_np=np.load(normal_filename + '.npy', mmap_mode=mmap_mode)
        if normals_np.dtype != np.float32:
            normals_np = normals_np.astype(np.float32)

        point_set=pts_np
        query_point_set=query_pts_np
        query_point_direction=direction_np       #:0:outside  1: inside
        normals = normals_np

        #point_set = np.loadtxt(fn[1]).astype(np.float32)
        #seg = np.loadtxt(fn[2]).astype(np.int64)
        #print(point_set.shape, seg.shape)

        #choice = np.random.choice(len(seg), self.npoints, replace=True)
        #resample
        #point_set = point_set[choice, :]

        #point_set = point_set - np.expand_dims(np.mean(point_set, axis = 0), 0) # center
        #dist = np.max(np.sqrt(np.sum(point_set ** 2, axis = 1)),0)
        #point_set = point_set / dist #scale

        if self.data_augmentation:
            theta = np.random.uniform(0,np.pi*2)
            rotation_matrix = np.array([[np.cos(theta), -np.sin(theta)],[np.sin(theta), np.cos(theta)]])
            point_set[:,[0,2]] = point_set[:,[0,2]].dot(rotation_matrix) # random rotation
            point_set += np.random.normal(0, 0.02, size=point_set.shape) # random jitter

        #seg = seg[choice]
        point_set = torch.from_numpy(point_set)
        query_pointset=torch.from_numpy(query_point_set)
        query_point_direction=torch.from_numpy(query_point_direction)
        normals=torch.from_numpy(normals)
        #seg = torch.from_numpy(seg)
        #cls = torch.from_numpy(np.array([cls]).astype(np.int64))

        #if self.classification:
        #    return point_set, cls
        #else:
        #    return point_set, seg
        return point_set,query_pointset,query_point_direction,normals

    def __len__(self):
        return len(self.shape_names)

class ModelNetDataset(data.Dataset):
    def __init__(self,
                 root,
                 npoints=2500,
                 split='train',
                 data_augmentation=True):
        self.npoints = npoints
        self.root = root
        self.split = split
        self.data_augmentation = data_augmentation
        self.fns = []
        with open(os.path.join(root, '{}.txt'.format(self.split)), 'r') as f:
            for line in f:
                self.fns.append(line.strip())

        self.cat = {}
        with open(os.path.join(os.path.dirname(os.path.realpath(__file__)), '../misc/modelnet_id.txt'), 'r') as f:
            for line in f:
                ls = line.strip().split()
                self.cat[ls[0]] = int(ls[1])

        print(self.cat)
        self.classes = list(self.cat.keys())

    def __getitem__(self, index):
        fn = self.fns[index]
        cls = self.cat[fn.split('/')[0]]
        with open(os.path.join(self.root, fn), 'rb') as f:
            plydata = PlyData.read(f)
        pts = np.vstack([plydata['vertex']['x'], plydata['vertex']['y'], plydata['vertex']['z']]).T
        choice = np.random.choice(len(pts), self.npoints, replace=True)
        point_set = pts[choice, :]

        point_set = point_set - np.expand_dims(np.mean(point_set, axis=0), 0)  # center
        dist = np.max(np.sqrt(np.sum(point_set ** 2, axis=1)), 0)
        point_set = point_set / dist  # scale

        if self.data_augmentation:
            theta = np.random.uniform(0, np.pi * 2)
            rotation_matrix = np.array([[np.cos(theta), -np.sin(theta)], [np.sin(theta), np.cos(theta)]])
            point_set[:, [0, 2]] = point_set[:, [0, 2]].dot(rotation_matrix)  # random rotation
            point_set += np.random.normal(0, 0.02, size=point_set.shape)  # random jitter

        point_set = torch.from_numpy(point_set.astype(np.float32))
        cls = torch.from_numpy(np.array([cls]).astype(np.int64))
        return point_set, cls


    def __len__(self):
        return len(self.fns)

if __name__ == '__main__':
    dataset = sys.argv[1]
    datapath = sys.argv[2]

    if dataset == 'shapenet':
        d = ShapeNetDataset(root = datapath, class_choice = ['Chair'])
        print(len(d))
        ps, seg = d[0]
        print(ps.size(), ps.type(), seg.size(),seg.type())

        d = ShapeNetDataset(root = datapath, classification = True)
        print(len(d))
        ps, cls = d[0]
        print(ps.size(), ps.type(), cls.size(),cls.type())
        # get_segmentation_classes(datapath)

    if dataset == 'modelnet':
        gen_modelnet_id(datapath)
        d = ModelNetDataset(root=datapath)
        print(len(d))
        print(d[0])

