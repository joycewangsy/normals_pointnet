
function ind = binarysearch(x, val)

if(val == x(1))
    ind = 1;
    return;
end

if(val == x(end))
    ind = length(x);
    return;
end

ind = [];
f = 1;
t = length(x);

while(f <= t)
    
    mid = round((f + t)/2);    
    
    if(x(mid) <= val && x(mid+1) > val)
        ind = mid;
        return;
    end
    
    if(x(mid) < val)
        f = mid+1;
    else
        t = mid-1;
    end
end




