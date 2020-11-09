function train_test_split

    shape_dir = '../data/shapes';    

    patches_per_shape = 5000;    
    seed = 590239578;

    if exist(shape_dir,'dir')~=7
        error(['Cannot find shape directory ',shape_dir]);
    end

    % (filename, name, has noise variations, has vardensity variations)
    trainset_shapes = { ...
        'fandisk','fandisk', true, true; ...
        'bunny','bunny', true, true; ...
        'armadillo','armadillo', true, true; ...
        'dragon_xyzrgb','dragon', true, true; ...
        'boxunion_uniform','boxunion', true, true; ...
        'tortuga','tortuga', true, true; ...
        'flower','flower', true, true; ...
        'Cup33','Cup33', true, true; ...
    };

    validationset_shapes = { ...
        'cube_uniform','cube', true, false; ...
        'galera','galera', true, true; ...
        'netsuke','netsuke', true, true; ...
    };    

    testset_shapes = { ...
        'cube_uniform','cube', true, false; ...
        'galera','galera', true, true; ...
        'icosahedron','icosahedron', true, true; ...
        'netsuke','netsuke', true, true; ...
        'Cup34','Cup34', true, true; ...
        'sphere','sphere', true, true; ...
        'cylinder','cylinder', true, true; ...
        'sphere_analytic','sphere_analytic', true, false; ...
        'cylinder_analytic','cylinder_analytic', true, false; ...
        'star_smooth','star_smooth', true, true; ...
        'star_halfsmooth','star_halfsmooth', true, true; ...
        'star_sharp','star_sharp', true, true; ...
        'Liberty','Liberty', true, true; ...
        'LegoLeg','LegoLeg', true, true; ...
        'boxunion2','boxunion2', true, true; ...
        'Boxy_smooth','Boxy_smooth', true, true; ...
        'box_groove','box_groove', true, true; ...
        'box_push','box_push', true, true; ...
        'pipe','pipe', true, true; ...
        'pipe_curve','pipe_curve', true, true; ...
        'column','column', true, true; ...
        'column_head','column_head', true, true; ...
        'sheet_analytic','sheet_analytic', true, false; ...
        'cube_analytic_smooth','cube_analytic_smooth', true, false; ...
        'cube_analytic_sharp','cube_analytic_sharp', true, false; ...
    };

    fullset_shapes = [trainset_shapes;testset_shapes];

    point_counts = {
        100000; ...
        };

    noise_types = {
        'no noise', 'white', 0; ...
        'whitenoise_low', 'white', 1e-2; ...
        'whitenoise_med', 'white', 5e-2; ...
        'whitenoise_high', 'white', 1e-1; ...
        };

    % (name, type)
    density_types = {
        'uniform', 'uniform'; ...
        'vardensity_minmax', 'minmax'; ...
        'vardensity_minmax_layers', 'minmax_layers'; ...
        };
    
    missingfiles = cell(1,0);
    
    % train
    disp('train sets ...');
    trainset_no_noise = shape_variations(trainset_shapes, point_counts, noise_types(1,:), density_types(1,:));
    missingfiles = [missingfiles,write_shape_set('trainingset_no_noise', shape_dir, trainset_no_noise, -1, seed)];
    
    trainset_whitenoise = [trainset_no_noise, ...
        shape_variations(trainset_shapes, point_counts, noise_types(2:end,:), density_types(1,:))];
    missingfiles = [missingfiles,write_shape_set('trainingset_whitenoise', shape_dir, trainset_whitenoise, -1, seed)];
    
    trainingset_vardensity = [trainset_no_noise, ...
        shape_variations(trainset_shapes, point_counts, noise_types(1,:), density_types(2:end,:))];
    missingfiles = [missingfiles,write_shape_set('trainingset_vardensity', shape_dir, trainingset_vardensity, -1, seed)];
    
    trainingset_full = [trainset_no_noise, ...
        shape_variations(trainset_shapes, point_counts, noise_types(2:end,:), density_types(1,:)), ...
        shape_variations(trainset_shapes, point_counts, noise_types(1,:), density_types(2:end,:))];
    missingfiles = [missingfiles,write_shape_set('trainingset_full', shape_dir, trainingset_full, -1, seed)];
    

    % validation
    disp('validation sets ...');
    validationset_no_noise = shape_variations(validationset_shapes, point_counts, noise_types(1,:), density_types(1,:));
    missingfiles = [missingfiles,write_shape_set('validationset_no_noise', shape_dir, validationset_no_noise, patches_per_shape, seed)];
    
    validationset_whitenoise = [validationset_no_noise, ...
        shape_variations(validationset_shapes, point_counts, noise_types(2:end,:), density_types(1,:))];
    missingfiles = [missingfiles,write_shape_set('validationset_whitenoise', shape_dir, validationset_whitenoise, patches_per_shape, seed)];
    
    validationset_vardensity = [validationset_no_noise, ...
        shape_variations(validationset_shapes, point_counts, noise_types(1,:), density_types(2:end,:))];
    missingfiles = [missingfiles,write_shape_set('validationset_vardensity', shape_dir, validationset_vardensity, patches_per_shape, seed)];
    
    validationset_full = [validationset_no_noise, ...
        shape_variations(validationset_shapes, point_counts, noise_types(2:end,:), density_types(1,:)), ...
        shape_variations(validationset_shapes, point_counts, noise_types(1,:), density_types(2:end,:))];
    missingfiles = [missingfiles,write_shape_set('validationset_full', shape_dir, validationset_full, patches_per_shape, seed)];
    
    
    % test
    disp('test sets ...');
    testset_no_noise = shape_variations(testset_shapes, point_counts, noise_types(1,:), density_types(1,:));
    missingfiles = [missingfiles,write_shape_set('testset_no_noise', shape_dir, testset_no_noise, patches_per_shape, seed)];
    
    testset_whitenoise = [testset_no_noise, ...
        shape_variations(testset_shapes, point_counts, noise_types(2:end,:), density_types(1,:))];
    missingfiles = [missingfiles,write_shape_set('testset_whitenoise', shape_dir, testset_whitenoise, patches_per_shape, seed)];
    
    testset_vardensity = [testset_no_noise, ...
        shape_variations(testset_shapes, point_counts, noise_types(1,:), density_types(2:end,:))];
    missingfiles = [missingfiles,write_shape_set('testset_vardensity', shape_dir, testset_vardensity, patches_per_shape, seed)];
    
    testset_full = [testset_no_noise, ...
        shape_variations(testset_shapes, point_counts, noise_types(2:end,:), density_types(1,:)), ...
        shape_variations(testset_shapes, point_counts, noise_types(1,:), density_types(2:end,:))];
    missingfiles = [missingfiles,write_shape_set('testset_full', shape_dir, testset_full, patches_per_shape, seed)];
    
    
    % combined
    disp('combined sets ...');
    fullset_no_noise = shape_variations(fullset_shapes, point_counts, noise_types(1,:), density_types(1,:));
    missingfiles = [missingfiles,write_shape_set('fullset_no_noise', shape_dir, fullset_no_noise, patches_per_shape, seed)];
    
    fullset_whitenoise = [fullset_no_noise, ...
        shape_variations(fullset_shapes, point_counts, noise_types(2:end,:), density_types(1,:))];
    missingfiles = [missingfiles,write_shape_set('fullset_whitenoise', shape_dir, fullset_whitenoise, patches_per_shape, seed)];
    
    fullset_vardensity = [fullset_no_noise, ...
        shape_variations(fullset_shapes, point_counts, noise_types(1,:), density_types(2:end,:))];
    missingfiles = [missingfiles,write_shape_set('fullset_vardensity', shape_dir, fullset_vardensity, patches_per_shape, seed)];
    
    fullset = [fullset_no_noise, ...
        shape_variations(fullset_shapes, point_counts, noise_types(2:end,:), density_types(1,:)), ...
        shape_variations(fullset_shapes, point_counts, noise_types(1,:), density_types(2:end,:))];
    missingfiles = [missingfiles,write_shape_set('fullset', shape_dir, fullset, patches_per_shape, seed)];
    
    
    if not(isempty(missingfiles))
        missingfiles = unique(missingfiles);
        warning('Some files were missing!');
        disp(missingfiles');
    else
        disp('All files found.');
    end
end

function missingfiles = write_shape_set(name, shape_dir, shape_variations, patches_per_shape, seed)
    
    for i=1:numel(shape_variations)
        if patches_per_shape > 0
            
%             % get 32 bit seed from hashed file name
%             hash = DataHash([shape_variations(i).filename],struct('Format','double', 'Method','SHA-1'));
%             seed = 0;
%             for j=1:4
%                 seed = seed + hash[j] * 2^(j-1);
%             end
            
            % select patch indices repeatably with the same seed (they are
            % always the same, but since the point order of a shape is
            % random, this should not matter)
            rng_orig_state = rng(seed);
            patch_indices = randsample(shape_variations(i).point_count, min(shape_variations(i).point_count, patches_per_shape), false)-1; % to zero-based indices
            rng(rng_orig_state);
            
            % write patch indices to .pidx file
            dlmwrite(fullfile(shape_dir,[shape_variations(i).filename,'.pidx']), patch_indices, 'precision', '%d', 'delimiter', ' ');
        end
    end
    
    missingfiles = cell(1,0);
    fid = fopen(fullfile(shape_dir,[name,'.txt']),'wt');
    for i=1:numel(shape_variations)
        if exist(fullfile(shape_dir,[shape_variations(i).filename,'.xyz']),'file') ~= 2
            missingfiles{end+1} = fullfile(shape_dir,[shape_variations(i).filename,'.xyz']); %#ok<AGROW>
        end
        if exist(fullfile(shape_dir,[shape_variations(i).filename,'.normals']),'file') ~= 2
            missingfiles{end+1} = fullfile(shape_dir,[shape_variations(i).filename,'.normals']); %#ok<AGROW>
        end
        if exist(fullfile(shape_dir,[shape_variations(i).filename,'.curv']),'file') ~= 2
            missingfiles{end+1} = fullfile(shape_dir,[shape_variations(i).filename,'.curv']); %#ok<AGROW>
        end
        if exist(fullfile(shape_dir,[shape_variations(i).filename,'.pidx']),'file') ~= 2
            missingfiles{end+1} = fullfile(shape_dir,[shape_variations(i).filename,'.pidx']); %#ok<AGROW>
        end
        fprintf(fid, [shape_variations(i).filename,'\n']);
    end
    fclose(fid);
end