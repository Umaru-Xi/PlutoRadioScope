img_size = 128;
phi = 29.1684;
theta = 275.32;
draw_r = 2;

img = zeros(img_size, img_size);
img_show = uint8(255 - img);
fig = imshow(img_show);
o_xy = floor(img_size / 2) + 1;
r = floor(phi / 90.0 * (o_xy));
a = theta * pi / 180;
x_c = img_size - (floor(r * cos(a)) + o_xy) + 1;
y_c = floor(r * sin(a)) + o_xy;

for ang = 0 : 0.01 : 2*pi
    x = floor(x_c + draw_r * cos(ang));
    y = floor(y_c + draw_r * sin(ang));
    if (x > 0) && (y > 0) && (x < img_size) && (y < img_size)
        img(x, y) = 255;
    end
end
img_show = uint8(255 - img);
fig = imshow(img_show);
imwrite(img_show, "octave_generated.bmp");