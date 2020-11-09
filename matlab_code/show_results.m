% show the pointclouds
clc;clear;close all;
vis = 1;
radius = 0.05;
evaluate_results_root = '../pcpnet-pytorch_0.3/results/my_saliency_0.15_0.012/';
data_root = '../saliency_test_models/';
testlist = 'train_files.txt';
test_name = importdata([data_root testlist]);
Total = 0;
for i=1:length(test_name)
    current_name = test_name{i};
    disp(current_name);
    points_gt_name = [data_root current_name '.xyz'];
%     saliency_gt_name= [data_root current_name '.saliency'];
    saliency_result_name = [evaluate_results_root current_name '.saliency'];
    points_gt = load(points_gt_name);
    bbdiag = norm(max(points_gt) -min(points_gt), 2);
    abs_radius = radius*bbdiag;
%     saliencys_gt = load(saliency_gt_name);

%     pidx_gt = load(pidx_gt_name);
%     for j=1:length(pidx_gt)
%         center = points_gt(pidx_gt(j),:);
%         center_normal = normals_gt(pidx_gt(j),:);
%         [idx,D] = rangesearch(points_gt,center,abs_radius);
%         patch = points_gt(idx{1:end},:);
%         patch_normal = normals_gt(idx{1:end},:);
%         delta1 = sqrt(sum((patch_normal-center_normal).^2,2));
%         delta2 = sqrt(sum((patch_normal+center_normal).^2,2));
%         delta = min(delta1,delta2);
%         delta = (delta-min(delta))/(max(delta)-min(delta));
%         delta(delta<0.5)=0;
%         delta(delta>=0.5)=1;
%         if vis==1
%             scatter3(center(:,1),center(:,2),center(:,3),'*');
%             hold on;
% %             scatter3(points_gt(:,1),points_gt(:,2),points_gt(:,3),'.');
% %             hold on;
%             s = 10*ones(size(delta));
%             scatter3(patch(:,1),patch(:,2),patch(:,3),s, delta,'filled');
%             axis equal;
%             axis off;
%         end
%     
%     end
    
    
    saliencys_result = load(saliency_result_name);
%     errors = abs(-saliencys_gt);

    if vis==1
        h = figure();
%         subplot(1,3,1);
%         s = ones(size(saliencys_gt));
%         scatter3(points_gt(:,1),points_gt(:,2),points_gt(:,3),s, saliencys_gt,'filled');
%         axis equal;
%         axis off;
%         subplot(1,3,2);
        s = ones(size(saliencys_result));
        scatter3(points_gt(:,1),points_gt(:,2),points_gt(:,3),s, saliencys_result,'filled')
        axis equal;
        axis off;
%         subplot(1,3,3);
%         scatter3(points_gt(:,1),points_gt(:,2),points_gt(:,3),s, errors,'filled')
%         axis equal;
%         axis off;
    end
end
disp(Total/length(test_name));
