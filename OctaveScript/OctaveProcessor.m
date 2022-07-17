data = load("Sun_10.7cm_unhead");
img_size = 128;

img = zeros(img_size, img_size);
img_cnt = ones(img_size, img_size);
img_sum = img;
img_show = uint8(255 - img);
o_xy = floor(img_size / 2) + 1;
fig = imshow(img_show);
max_i = size(data);
max_i = max_i(1);

for i = 1 : max_i
    buffer_matrix = data(i, :);
    freq = buffer_matrix(1);
    pwr = buffer_matrix(2);
    theta = buffer_matrix(3);
    phi = buffer_matrix(4);
    if(phi > 90.0)
        continue;
    end
    r = floor(phi / 90.0 * (o_xy));
    a = theta * pi / 180;
    x = img_size - (floor(r * cos(a)) + o_xy) + 1;
    y = floor(r * sin(a)) + o_xy;
    img_sum(x, y) = img_sum(x, y) + pwr;
    img_cnt(x, y) = img_cnt(x, y) + 1;
    img = img_sum ./ img_cnt;
    max_img = max(max(img));
    min_img = min(min(img));
    k_scale = 255/(max_img - min_img);
    img_tmp = floor((img - min_img) * k_scale);
    img_show = uint8(255 - img_tmp);
end
% img_avg = sum(sum(img_show))./(img_size*img_size);
% img_show(img_show >= img_avg) = 255;
% img_show(img_show < img_avg) = 0;
fig = imshow(uint8(img_show));
imwrite(img_show, 'octave_processed.bmp');
