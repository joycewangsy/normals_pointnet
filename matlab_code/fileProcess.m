%path:需要处理的文件夹路径，注意最后加上“\”，否则读不到文件
%suffix:特定的后缀名
function filePath = fileProcess(path,suffix)
files = dir([path,'*.',suffix]);
len = length(files);
for i = 1:len
    filePath = [path,files(i).name]; %得到文件路径
    %文件处理部分
end