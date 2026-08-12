关于本文件夹的JSON格式标准：

background.json: 关于背景图片的设置
{
  "bg_color": [255, 255, 255], // 背景颜色，RGB格式，范围0-255
  "max_scale": 1000.0 // 背景图片的最大缩放比例，单位为倍数
}

pen_settings.json: 关于画笔的设置
{
  "pen_color": [255, 255, 255], // 画笔颜色，RGB格式，范围0-255
  "pen_width": 1.0 // 画笔宽度，单位为像素
}

run_mode.json: 关于运行模式的设置
{
    "mode": "normal" // 运行模式，可选值：normal（正常模式，会出现一个可以调整尺寸的标准窗口）、full（全屏模式[默认]）
}