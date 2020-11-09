from __future__ import print_function
import argparse
import os
import random
import torch
import torch.nn.parallel
import torch.optim as optim
import torch.utils.data
from pointnet.dataset import normalDataset
from pointnet.model import im_normal_network, feature_transform_regularizer
import torch.nn.functional as F
from tqdm import tqdm
import numpy as np


parser = argparse.ArgumentParser()
parser.add_argument(
    '--batchSize', type=int, default=2, help='input batch size')
parser.add_argument(
    '--workers', type=int, help='number of data loading workers', default=4)
parser.add_argument(
    '--nepoch', type=int, default=25, help='number of epochs to train for')
parser.add_argument('--indir', type=str, default='/Users/apple/code/normalpointnet.pytorch-master/dataset', help='input folder')
parser.add_argument('--outf', type=str, default='/Users/apple/code/normalpointnet.pytorch-master/models', help='output folder')
parser.add_argument('--model', type=str, default='', help='model path')
parser.add_argument('--trainset', type=str, default='train_files.txt', help='training set file name')
parser.add_argument('--testset', type=str, default='train_files.txt', help='test set file name')
#parser.add_argument('--dataset', type=str, required=True, help="dataset path")
#parser.add_argument('--class_choice', type=str, default='Chair', help="class_choice")
parser.add_argument('--feature_transform', action='store_true', help="use feature transform")

opt = parser.parse_args()
print(opt)

opt.manualSeed = random.randint(1, 10000)  # fix seed
print("Random Seed: ", opt.manualSeed)
random.seed(opt.manualSeed)
torch.manual_seed(opt.manualSeed)

dataset = normalDataset(
    root=opt.indir,
    shape_list_filename=opt.trainset,
    npoints=2048,
    data_augmentation=True)
dataloader = torch.utils.data.DataLoader(
    dataset,
    batch_size=opt.batchSize,
    shuffle=True,
    num_workers=int(opt.workers))

test_dataset = normalDataset(
    root=opt.indir,
    shape_list_filename=opt.trainset,
    npoints=2048,
    data_augmentation=True)
testdataloader = torch.utils.data.DataLoader(
    test_dataset,
    batch_size=opt.batchSize,
    shuffle=True,
    num_workers=int(opt.workers))
print(len(dataset), len(test_dataset))
#num_classes = dataset.num_seg_classes
#print('classes', num_classes)
try:
    os.makedirs(opt.outf)
except OSError:
    pass

blue = lambda x: '\033[94m' + x + '\033[0m'

#classifier = PointNetDenseCls(k=num_classes, feature_transform=opt.feature_transform)
classifier=im_normal_network(1,1024,3)
if opt.model != '':
    classifier.load_state_dict(torch.load(opt.model))

optimizer = optim.Adam(classifier.parameters(), lr=0.001, betas=(0.9, 0.999))
scheduler = optim.lr_scheduler.StepLR(optimizer, step_size=20, gamma=0.5)
#classifier.cuda()

num_batch = len(dataset) / opt.batchSize

for epoch in range(opt.nepoch):
    scheduler.step()
    for i, data in enumerate(dataloader, 0):
        #points, target = data
        avg_loss_sp = 0
        avg_num = 0
        points, query_points, query_points_direction, normals=data
        points = points.transpose(2, 1)
        #points, target = points.cuda(), target.cuda()
        optimizer.zero_grad()
        classifier = classifier.train()
        #pred, trans, trans_feat = classifier(points)
        out,trans_feat = classifier(points, query_points)
        #pred = pred.view(-1, num_classes)
        #target = target.view(-1, 1)[:, 0] - 1
        #print(pred.size(), target.size())
        loss = torch.mean((out-query_points_direction)**2)
        print(loss)
        print(loss.item())
        if opt.feature_transform:
            loss += feature_transform_regularizer(trans_feat) * 0.001
        loss.backward()
        optimizer.step()
        avg_loss_sp += loss.item()
        avg_num += 1
        #print('[%d: %d/%d] train loss: %f accuracy: %f' % (epoch, i, num_batch, loss.item(), correct.item()/float(opt.batchSize * 2500)))
        print('[%d: %d/%d] train loss: %f loss_sp: %f' % (epoch, i, num_batch, loss.item(), avg_loss_sp/avg_num))
        if i % 10 == 0:
            j, data = next(enumerate(testdataloader, 0))
            test_avg_loss_sp = 0
            test_avg_num = 0
            #points, target = data
            points, query_points, query_points_direction, normals = data
            points = points.transpose(2, 1)
            #points, target = points.cuda(), target.cuda()
            classifier = classifier.eval()
            #pred, _, _ = classifier(points)
            out, trans_feat = classifier(points, query_points)
            #pred = pred.view(-1, num_classes)
            #target = target.view(-1, 1)[:, 0] - 1
            #loss = F.nll_loss(pred, target)
            loss = torch.mean((out - query_points_direction) ** 2)
            #pred_choice = pred.data.max(1)[1]
            #correct = pred_choice.eq(target.data).cpu().sum()
            #print('[%d: %d/%d] %s loss: %f accuracy: %f' % (epoch, i, num_batch, blue('test'), loss.item(), correct.item()/float(opt.batchSize * 2500)))
            test_avg_loss_sp += loss.item()
            test_avg_num += 1
            # print('[%d: %d/%d] train loss: %f accuracy: %f' % (epoch, i, num_batch, loss.item(), correct.item()/float(opt.batchSize * 2500)))
            print('[%d: %d/%d] loss: %f loss_sp: %f' % (epoch, i, num_batch, loss.item(), test_avg_loss_sp / test_avg_num))
    torch.save(classifier.state_dict(), '%s/normal_model_%d.pth' % (opt.outf, epoch))

