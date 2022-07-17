fprintf("Octave client launched.");

pkg load instrument-control
img_scale = 15;

octave_client = tcpclient("127.0.0.1", octave_server_port);

img = zeros(img_size, img_size);
img_cnt = ones(img_size, img_size);
img_sum = img;
img_show = uint8(255 - img);
o_xy = floor(img_size / 2) + 1;
fig = imshow(img_show);

while(1)
    write(octave_client, "Get");
    pause(0.01);
    if(octave_client.NumBytesAvailable != 0)
    buffer = read(octave_client, octave_client.NumBytesAvailable);
    buffer_matrix = str2num(char(buffer));
    freq = buffer_matrix(1);
    pwr = buffer_matrix(2);
    theta = buffer_matrix(3);
    phi = buffer_matrix(4);
    if(phi > 90.0)
        continue;
    end
    fprintf("%s", buffer);
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
    fig = imshow(img_show);
    end
end
