%path:��Ҫ������ļ���·����ע�������ϡ�\��������������ļ�
%suffix:�ض��ĺ�׺��
function filePath = fileProcess(path,suffix)
files = dir([path,'*.',suffix]);
len = length(files);
for i = 1:len
    filePath = [path,files(i).name]; %�õ��ļ�·��
    %�ļ�������
end