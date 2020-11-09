function [ selected ] = farthest_points( points, m, fixed_points )

    N = size(points,1);
    
    if ~isempty(fixed_points)
        min_distances = (min(pdist2(points, fixed_points), [], 2))';        
        [~,selection] = max(min_distances);
    else   
        min_distances = inf(1, N);
        selection = 1;
    end
    
    selected = false(1, N);
    
    for i = 1:min(N+1, m)
        selected(selection) = true;
        
        selection_distances = zeros(1, N);
        not_selected = ~selected;
        num_not_selected = sum(not_selected);
        selection_distances(~selected) = sum((points(~selected, :) - repmat(points(selection, :), [num_not_selected,1])).^2,2);
        min_distances = min(min_distances, selection_distances);
        
        [~,selection] = max(min_distances);        
    end

end
