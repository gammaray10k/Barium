#include "ui_save_as_ui.h"
#include "ui_select_source_ui.h"
#include "ui_video_ui.h"
#include "video_ui.h"

#include <QComboBox>
#include <QDial>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QRadioButton>
#include <QSlider>
#include <QSpinBox>


VideoUI::VideoUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VideoUI)
{
    ui->setupUi(this);

    //connect signals and slots
    connect(ui->videoRFSlider, &QSlider::valueChanged,
            this, &VideoUI::select_crf);
    connect(ui->videoRFSlider, &QSlider::valueChanged,
            this, &VideoUI::select_qscale);
    connect(ui->videoRFSlider, &QSlider::valueChanged,
            ui->videoRFSpinBox, &QSpinBox::setValue);
    connect(ui->videoEncoderDial, &QDial::valueChanged,
            this, &VideoUI::encoder_preset);
    //-------------------------------------------------------------------------
    connect(ui->videoCodecBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &VideoUI::select_vid_codec);
    connect(ui->videoCodecBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &VideoUI::vid_codec_interface);
    connect(ui->videoContainerBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &VideoUI::select_container);/**/
    connect(ui->videoResBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &VideoUI::select_vid_res);
    connect(ui->videoAspectRatBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &VideoUI::select_aspect_rat);
    connect(ui->videoFPSBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &VideoUI::select_vid_fps);
    connect(ui->videoMotionESTBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &VideoUI::select_motion_est);
    //-------------------------------------------------------------------------


    //Initialize interface widgets
    //additional radio button defaults
    rateOptButtons = new QButtonGroup(this);
    rateOptButtons->addButton(ui->videoCRFRadio);
    rateOptButtons->addButton(ui->videoAVGBitRadio);
    rateOptButtons->setExclusive(true);
    ui->videoCRFRadio->setChecked(true);

    //constant rf spinbox settings
    ui->videoRFSpinBox->setAlignment(Qt::AlignHCenter);
    ui->videoRFSpinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    ui->videoRFSpinBox->setFrame(false);
    ui->videoRFSpinBox->setRange(0, 51);
    ui->videoRFSpinBox->setValue(23); //default value for the spinBox
    ui->videoRFSpinBox->setReadOnly(true);

    //slider
    ui->videoRFSlider->setRange(0, 51);
    ui->videoRFSlider->setTickPosition(QSlider::TicksBelow);
    ui->videoRFSlider->setSingleStep(1);
    ui->videoRFSlider->setValue(23);//default value for the slider
    ui->videoRFSlider->setSliderPosition(23);
    ui->videoRFSlider->setToolTip(tr("Constant Rate Factor Selector"));
    crf_value.setNum(ui->videoRFSlider->value());//send default to encoder

    //encoder dial
    ui->videoDialPreset->setAlignment(Qt::AlignCenter);
    ui->videoEncoderDial->setRange(0, 9);
    ui->videoEncoderDial->setNotchesVisible(true);
    ui->videoEncoderDial->setSingleStep(1);
    ui->videoEncoderDial->setValue(4);//setting default value
    ui->videoEncoderDial->setSliderPosition(4);
    ui->videoEncoderDial->setToolTip(tr("default: medium encoding"));
    ui->videoDialPreset->setText(tr("medium"));//default value
    pr_value = "medium";//send default to encoder
    //ui->videoEncoderDial->setInvertedAppearance(true);

    //video codec interface
    ui->videoCodecBox->insertItem(0, "Source");
    ui->videoCodecBox->insertSeparator(1);
    videoCodecList << "x264" << "x265" << "Xvid" << "VP9" << "Theora"
                   << "MPEG-1" << "MPEG-2" << "RAV1E/AV1";
    ui->videoCodecBox->insertItems(2, videoCodecList);

    //video container
    ui->videoContainerBox->insertItem(0, "Source");
    ui->videoContainerBox->insertSeparator(1);
    videoContainerList << "MP4" << "MKV" << "WebM" << "TS" << "OGV";
    ui->videoContainerBox->insertItems(2, videoContainerList);

    //video resolution
    ui->videoResBox->insertItem(0, "Source");
    ui->videoResBox->insertSeparator(1);
    videoResList << "640x360" << "720x480" << "720x576" << "1024x768"
                 << "1280x720" << "1366x768" << "1600x900" << "1920x1080"
                 << "1920x1200" << "2048x1080" << "2048x1152" << "2560x1440"
                 << "3840x2160" << "4096x2160";
    ui->videoResBox->insertItems(2, videoResList);

    //video aspect ratio
    ui->videoAspectRatBox->insertItem(0, "Source");
    ui->videoAspectRatBox->insertSeparator(1);
    videoAspecRatList << "4:3" << "16:9" << "21:9";
    ui->videoAspectRatBox->insertItems(2, videoAspecRatList);

    //video framerate
    ui->videoFPSBox->insertItem(0, "Source");
    ui->videoFPSBox->insertSeparator(1);
    videoFPSList << "23.976" << "24" << "25" << "29.97" << "30" << "60" << "120";
    ui->videoFPSBox->insertItems(2, videoFPSList);
    //ui->videoFPSBox->setCurrentIndex(5);--->test, it works!

    //video motion estimation method/motion est
    ui->videoMotionESTBox->insertItem(0, "Default");
    ui->videoMotionESTBox->insertSeparator(1);
    videoMotESTList << "ME_DIA" << "ME_HEX" << "ME_UMH" << "ME_ESA" << "ME_TESA";
    ui->videoMotionESTBox->insertItems(2, videoMotESTList);
    ui->videoMotionESTBox->setToolTip(tr("Default = ME_DIA"));

    //video average bitrate
    ui->videoAVGBitRadio->setText(tr("Avg Bitrate (Kbps)"));
    ui->videoCRFRadio->setText(tr("Constant Rate Factor"));

    //video groupbox
    ui->videoGroupBox->setTitle(tr("Video"));
    ui->videoGroupBox->setAlignment(Qt::AlignLeft);

}

VideoUI::~VideoUI()
{
    delete ui;
}

//ui->videoEncoderDial->setRange(0, 9);
void VideoUI::encoder_preset()
{
    if(ui->videoEncoderDial->value() == 0 &&
            ui->videoEncoderDial->sliderPosition() == 0)
    {
        //test for AV1 codec/rav1e
        if(ui->videoCodecBox->currentIndex() == 9)
        {
            ui->videoDialPreset->setText(tr("Speed 0"));
            pr_value = "0";
            ui->videoEncoderDial->setToolTip(tr("Speed 0 Encoding"));
        }
        //any other video codec
        else
        {
            ui->videoDialPreset->setText(tr("placebo"));
            pr_value = "placebo";
            ui->videoEncoderDial->setToolTip(tr("placebo encoding"));
            enc_preset = preset + pr_value;
        }
    }
    if(ui->videoEncoderDial->value() == 1 &&
            ui->videoEncoderDial->sliderPosition() == 1)
    {
        //test for AV1/rav1e
        if(ui->videoCodecBox->currentIndex() == 9)
        {
            ui->videoDialPreset->setText(tr("Speed 1"));
            pr_value = "1";
            ui->videoEncoderDial->setToolTip(tr("Speed 1 Encoding"));
        }
        //any other video codec
        else
        {
            ui->videoDialPreset->setText(tr("veryslow"));
            pr_value = "veryslow";
            ui->videoEncoderDial->setToolTip(tr("veryslow encoding"));
            enc_preset = preset + pr_value;
        }
    }
    if(ui->videoEncoderDial->value() == 2 &&
            ui->videoEncoderDial->sliderPosition() == 2)
    {
        //test for AV1/rav1e
        if(ui->videoCodecBox->currentIndex() == 9)
        {
            ui->videoDialPreset->setText(tr("Speed 2"));
            pr_value = "2";
            ui->videoEncoderDial->setToolTip(tr("Speed 2 Encoding"));
        }
        //any other video codec
        else
        {
            ui->videoDialPreset->setText(tr("slower"));
            pr_value = "slower";
            ui->videoEncoderDial->setToolTip(tr("slower encoding"));
            enc_preset = preset + pr_value;
        }
    }
    if(ui->videoEncoderDial->value() == 3 &&
            ui->videoEncoderDial->sliderPosition() == 3)
    {
        //test for AV1/rav1e
        if(ui->videoCodecBox->currentIndex() == 9)
        {
            ui->videoDialPreset->setText(tr("Speed 3"));
            pr_value = "3";
            ui->videoEncoderDial->setToolTip(tr("Speed 3 Encoding"));
        }
        //any other video codec
        else
        {
            ui->videoDialPreset->setText(tr("slow"));
            pr_value = "slow";
            ui->videoEncoderDial->setToolTip(tr("slow encoding"));
            enc_preset = preset + pr_value;
        }
    }
    if(ui->videoEncoderDial->value() == 4 &&
            ui->videoEncoderDial->sliderPosition() == 4)
    {
        //test for AV1/rav1e
        if(ui->videoCodecBox->currentIndex() == 9)
        {
            ui->videoDialPreset->setText(tr("Speed 4"));
            pr_value = "4";
            ui->videoEncoderDial->setToolTip(tr("Speed 4 Encoding"));
        }
        //any other video codec
        else
        {
            ui->videoDialPreset->setText(tr("medium"));
            pr_value = "medium";
            ui->videoEncoderDial->setToolTip(tr("medium encoding"));
            enc_preset = preset + pr_value;
        }
    }
    if(ui->videoEncoderDial->value() == 5 &&
            ui->videoEncoderDial->sliderPosition() == 5)
    {
        //test for AV1/rav1e
        if(ui->videoCodecBox->currentIndex() == 9)
        {
            ui->videoDialPreset->setText(tr("Speed 5"));
            pr_value = "5";
            ui->videoEncoderDial->setToolTip(tr("Speed 5 Encoding"));
        }
        //any other video codec
        else
        {
            ui->videoDialPreset->setText(tr("fast"));
            pr_value = "fast";
            ui->videoEncoderDial->setToolTip(tr("fast encoding"));
            enc_preset = preset + pr_value;
        }
    }
    if(ui->videoEncoderDial->value() == 6 &&
            ui->videoEncoderDial->sliderPosition() == 6)
    {
        //test for AV1/rav1e
        if(ui->videoCodecBox->currentIndex() == 9)
        {
            ui->videoDialPreset->setText(tr("Speed 6"));
            pr_value = "6";
            ui->videoEncoderDial->setToolTip(tr("Speed 6 Encoding"));
        }
        //any other video codec
        else
        {
            ui->videoDialPreset->setText(tr("faster"));
            pr_value = "faster";
            ui->videoEncoderDial->setToolTip(tr("faster encoding"));
            enc_preset = preset + pr_value;
        }
    }
    if(ui->videoEncoderDial->value() == 7 &&
            ui->videoEncoderDial->sliderPosition() == 7)
    {
        //test for AV1/rav1e
        if(ui->videoCodecBox->currentIndex() == 9)
        {
            ui->videoDialPreset->setText(tr("Speed 7"));
            pr_value = "7";
            ui->videoEncoderDial->setToolTip(tr("Speed 7 Encoding"));
        }
        //any other video codec
        else
        {
            ui->videoDialPreset->setText(tr("veryfast"));
            pr_value = "veryfast";
            ui->videoEncoderDial->setToolTip(tr("veryfast encoding"));
            enc_preset = preset + pr_value;
        }
    }
    if(ui->videoEncoderDial->value() == 8 &&
            ui->videoEncoderDial->sliderPosition() == 8)
    {
        //test for AV1/rav1e
        if(ui->videoCodecBox->currentIndex() == 9)
        {
            ui->videoDialPreset->setText(tr("Speed 8"));
            pr_value = "8";
            ui->videoEncoderDial->setToolTip(tr("Speed 8 Encoding"));
        }
        //any other codec
        else
        {
            ui->videoDialPreset->setText(tr("superfast"));
            pr_value = "superfast";
            ui->videoEncoderDial->setToolTip(tr("superfast encoding"));
            enc_preset = preset + pr_value;
        }
    }
    if(ui->videoEncoderDial->value() == 9 &&
            ui->videoEncoderDial->sliderPosition() == 9)
    {
        //test for AV1/rav1e
        if(ui->videoCodecBox->currentIndex() == 9)
        {
            ui->videoDialPreset->setText(tr("Speed 9"));
            pr_value = "9";
            ui->videoEncoderDial->setToolTip(tr("Speed 9 Encoding"));
        }
        //any other codec
        else
        {
            ui->videoDialPreset->setText(tr("ultrafast"));
            pr_value = "ultrafast";
            ui->videoEncoderDial->setToolTip(tr("ultrafast encoding"));
            enc_preset = preset + pr_value;
        }
    }
    if(ui->videoEncoderDial->value() == 10 &&
            ui->videoEncoderDial->sliderPosition() == 10)//<==Speed 10 does not appear in application
    {
        //test for AV1/rav1e
        if(ui->videoCodecBox->currentIndex() == 9)
        {
            ui->videoDialPreset->setText(tr("Speed 10"));
            pr_value = "10";
            ui->videoEncoderDial->setToolTip(tr("Speed 10 Encoding"));
        }
        else
        {
            ui->videoEncoderDial->setRange(0, 9);
        }
    }
    //test, it works!
    //QString str = "";
    //ui->videoDialPreset->setText("placebo " +
    //str.setNum(ui->videoEncoderDial->value()));
}

//initalizing selected crf value
void VideoUI::select_crf()
{
    //crf_value is part of input for ffmpeg args in encoding_started()
    //setting the crf value to string
    crf_value.setNum(ui->videoRFSlider->value());
}

//initalizing selected qscale value
void VideoUI::select_qscale()
{
    //setting qscale value to string
    video_qs_value.setNum(ui->videoRFSlider->value());
}

void VideoUI::select_vid_codec()
{
    //select video codec
    if(ui->videoCodecBox->currentIndex() == 0)
    {
        //copy
        video_codec = "copy";

    }
    if(ui->videoCodecBox->currentIndex() == 2)
    {
        //x264
        video_codec = "libx264";
    }
    if(ui->videoCodecBox->currentIndex() == 3)
    {
        //x265
        video_codec = "libx265";
    }
    if(ui->videoCodecBox->currentIndex() == 4)
    {
        //Xvid
        video_codec = "libxvid";
    }
    if(ui->videoCodecBox->currentIndex() == 5)
    {
        //vp9
        video_codec = "libvpx-vp9";
    }
    if(ui->videoCodecBox->currentIndex() == 6)
    {
        //theora
        video_codec = "libtheora";
    }
    if(ui->videoCodecBox->currentIndex() == 7)
    {
        //MPEG-1
        video_codec = "mpeg1video";
    }
    if(ui->videoCodecBox->currentIndex() == 8)
    {
        //MPEG-2
        video_codec = "mpeg2video";
    }
    if(ui->videoCodecBox->currentIndex() == 9)
    {
        //rav1e
        video_codec = "librav1e";
    }
    /*else
    {
        video_codec = ui->videoCodecBox->currentText().toLower();
    }*/
}

void VideoUI::vid_codec_interface()
{
    //video codec interface settings
    if(ui->videoCodecBox->currentIndex() == 2)
    {
        //default libx264 crf value is 23
        ui->videoCRFRadio->setText(tr("Constant Rate Factor"));
        ui->videoRFSpinBox->setRange(0, 51);
        ui->videoRFSlider->setRange(0, 51);
        ui->videoRFSlider->setValue(23);
        ui->videoRFSpinBox->setValue(23);
        ui->videoRFSlider->setSliderPosition(23);
        ui->videoLQLabel->setText(tr("Low Quality |"));
        ui->videoHQLabel->setText(tr("| High Quality"));
        ui->videoEncoderDial->setRange(0, 9);
    }
    else if(ui->videoCodecBox->currentIndex() == 3)
    {
        //default libx265 crf value is 28
        ui->videoCRFRadio->setText(tr("Constant Rate Factor"));
        ui->videoRFSpinBox->setRange(0, 51);
        ui->videoRFSlider->setRange(0, 51);
        ui->videoRFSlider->setValue(28);
        ui->videoRFSpinBox->setValue(28);
        ui->videoRFSlider->setSliderPosition(28);
        ui->videoLQLabel->setText(tr("Low Quality |"));
        ui->videoHQLabel->setText(tr("| High Quality"));
        ui->videoEncoderDial->setRange(0, 9);
    }
    else if(ui->videoCodecBox->currentIndex() == 4)
    {
        //default Xvid settings
        ui->videoRFSlider->setTickPosition(QSlider::TicksBelow);
        ui->videoRFSpinBox->setRange(1, 31);
        ui->videoRFSlider->setRange(1, 31);
        ui->videoRFSlider->setValue(12);
        ui->videoRFSpinBox->setValue(12);
        ui->videoRFSlider->setSliderPosition(12);
        ui->videoCRFRadio->setText(tr("Variable Bit Rate"));
        ui->videoLQLabel->setText(tr("Low Quality |"));
        ui->videoHQLabel->setText(tr("| High Quality"));
        ui->videoEncoderDial->setRange(0, 9);
        video_qs_value.setNum(ui->videoRFSlider->value());//setting qscale value
    }
    else if(ui->videoCodecBox->currentIndex() == 5)
    {
        //default libvpx-vp9 crf value is 31, range is 0-63
        ui->videoCRFRadio->setText(tr("Constant Rate Factor"));
        ui->videoRFSpinBox->setRange(0, 63);
        ui->videoRFSlider->setRange(0, 63);
        ui->videoRFSlider->setValue(31);
        ui->videoRFSpinBox->setValue(31);
        ui->videoRFSlider->setSliderPosition(31);
        ui->videoLQLabel->setText(tr("Low Quality |"));
        ui->videoHQLabel->setText(tr("| High Quality"));
        ui->videoEncoderDial->setRange(0, 9);
        video_br_value = "0";
    }
    else if(ui->videoCodecBox->currentIndex() == 6)
    {
        //default libtheora settings, range is 0-10, 10 is higest quality
        ui->videoRFSlider->setTickPosition(QSlider::TicksBelow);
        ui->videoRFSpinBox->setRange(0, 10);
        ui->videoRFSlider->setRange(0, 10);
        ui->videoRFSlider->setValue(7);
        ui->videoRFSpinBox->setValue(7);
        ui->videoRFSlider->setSliderPosition(7);
        ui->videoRFSlider->setSingleStep(1);
        ui->videoCRFRadio->setText(tr("Variable Bit Rate"));
        ui->videoLQLabel->setText(tr("High Quality |"));
        ui->videoHQLabel->setText(tr("| Low Quality"));
        ui->videoEncoderDial->setRange(0, 9);
        video_qs_value.setNum(ui->videoRFSlider->value());//setting qscale value
    }
    else if(ui->videoCodecBox->currentIndex() == 7 ||
            ui->videoCodecBox->currentIndex() == 8)
    {
        //default mpeg1 and mpeg2 settimgs, range is 1-31, 1 is highest
        ui->videoRFSlider->setTickPosition(QSlider::TicksBelow);
        ui->videoRFSpinBox->setRange(1, 31);
        ui->videoRFSlider->setRange(1, 31);
        ui->videoRFSlider->setValue(5);
        ui->videoRFSpinBox->setValue(5);
        ui->videoRFSlider->setSliderPosition(5);
        ui->videoRFSlider->setSingleStep(1);
        ui->videoCRFRadio->setText(tr("Quality Rate Factor"));
        ui->videoLQLabel->setText(tr("Low Quality |"));
        ui->videoHQLabel->setText(tr("| High Quality"));
        ui->videoEncoderDial->setRange(0, 9);
        video_qs_value.setNum(ui->videoRFSlider->value());//setting qscale value
    }
    else if(ui->videoCodecBox->currentIndex() == 9)
    {
        //default rav1e settings, qp range is 0-255, 0 is highest
        ui->videoRFSlider->setTickPosition(QSlider::TicksBelow);
        ui->videoRFSpinBox->setRange(0, 255);
        ui->videoRFSlider->setRange(0, 255);
        ui->videoEncoderDial->setRange(0, 10);
        ui->videoRFSlider->setValue(100);
        ui->videoRFSpinBox->setValue(100);
        ui->videoRFSlider->setSliderPosition(100);
        ui->videoRFSlider->setSingleStep(1);
        ui->videoCRFRadio->setText(tr("Quantizer Factor"));
        ui->videoLQLabel->setText(tr("Low Quality |"));
        ui->videoHQLabel->setText(tr("| High Quality"));
        ui->videoEncoderDial->setToolTip(tr("default: Speed 4 Encoding"));
        ui->videoDialPreset->setText(tr("Speed 4"));//default value
        video_qs_value.setNum(ui->videoRFSlider->value());//setting qp value
    }
    else
    {
        ui->videoCRFRadio->setText(tr("Constant Rate Factor"));
        ui->videoRFSlider->setRange(0, 51);
        ui->videoRFSpinBox->setRange(0, 51);
        ui->videoRFSlider->setValue(23);
        ui->videoRFSpinBox->setValue(23);
        ui->videoRFSlider->setSingleStep(1);
        ui->videoRFSlider->setSliderPosition(23);
        ui->videoLQLabel->setText(tr("Low Quality |"));
        ui->videoHQLabel->setText(tr("| High Quality"));
        ui->videoEncoderDial->setRange(0, 9);
        ui->videoEncoderDial->setToolTip(tr("default: medium encoding"));
        ui->videoDialPreset->setText(tr("medium"));//default value
    }
}

void VideoUI::select_container()//fixed!!!
{
    /*connect(&SUI, &SaveAsUI::saveas_value,
            this, &VideoUI::select_container);//step closer!*/
    output_file = SUI.ui->saveASEdit->text();
    //based on code from qtffmpeg
    output_file = SUI.ui->saveASEdit->text();//get the current text on the field
    output_file = output_file.left(output_file.lastIndexOf("."));

    if(ui->videoContainerBox->currentIndex() == 0)
    {
        //copy source file extension
        QString source_ext{};
        QString input_file1_str = SSUI.ui->sourceInput1Edit->text();
        source_ext = input_file1_str.mid(input_file1_str.lastIndexOf("."));
        output_file.append(source_ext);
        SUI.ui->saveASEdit->setText(output_file);
        vid_ext = source_ext;
    }
    if(ui->videoContainerBox->currentIndex() == 2)
    {
        //mp4
        output_file.append(".mp4");
        SUI.ui->saveASEdit->setText(output_file);
        vid_ext = "mp4";
    }//gui not changing based on selection above. you made changes to SaveASUI and SelectSourceUI
    /*if(ui->videoContainerBox->currentIndex() == 3)
    {
        //mkv
        output_file.append(".mkv");
        ui->saveASEdit->setText(output_file);
        vid_ext = "mkv";
    }
    if(ui->videoContainerBox->currentIndex() == 4)
    {
        //webm
        output_file.append(".webm");
        ui->saveASEdit->setText(output_file);
        vid_ext = "webm";
    }
    if(ui->videoContainerBox->currentIndex() == 5)
    {
        //ts
        output_file.append(".ts");
        ui->saveASEdit->setText(output_file);
        vid_ext = "ts";
    }
    if(ui->videoContainerBox->currentIndex() == 6)
    {
        //ogv
        output_file.append(".ogv");
        ui->saveASEdit->setText(output_file);
        vid_ext = "ogv";
    }*/
}

void VideoUI::select_vid_res()
{
    //select video resolution
    if(ui->videoResBox->currentIndex() == 0)//it works!
    {
        //input1_vid_res holds the default res value for input_file1
        video_res_value = "copy";
    }
    else
    {
        video_res_value = "scale="+ui->videoResBox->currentText();
    }
}

void VideoUI::select_aspect_rat()
{
    if(ui->videoAspectRatBox->currentIndex() == 0)
    {
        vid_aspect_val = "copy";
    }
    else
    {
        vid_aspect_val = ui->videoAspectRatBox->currentText();
    }
}

void VideoUI::select_vid_fps()
{
    if(ui->videoFPSBox->currentIndex() == 0)
    {
        video_fps_val = "copy";
    }
    else
    {
        video_fps_val = ui->videoFPSBox->currentText();
    }
}

void VideoUI::select_motion_est()
{
    //ME (Motion Estimate) Method/Motion EST
    if(ui->videoMotionESTBox->currentIndex() == 0)
    {
        //default: DIA is an alias for EPZS; x264's default in FFmpeg
        video_motion_val = "dia";
    }
    if(ui->videoMotionESTBox->currentIndex() == 1)
    {
        //dia
        video_motion_val = "dia";
    }
    if(ui->videoMotionESTBox->currentIndex() == 2)
    {
        //hex
        video_motion_val = "hex";
    }
    if(ui->videoMotionESTBox->currentIndex() == 3)
    {
        //umh
        video_motion_val = "umh";
    }
    if(ui->videoMotionESTBox->currentIndex() == 4)
    {
        //esa
        video_motion_val = "esa";
    }
    if(ui->videoMotionESTBox->currentIndex() == 5)
    {
        //tesa
        video_motion_val = "tesa";
    }
}

//average bitrate field
//maybe make it a slot??
QString VideoUI::get_vid_bitrate_field_text()
{
    if(!ui->videoAVGBitField->text().isEmpty())
    {
        video_bitrate = ui->videoAVGBitField->text()+"k";
    }
    else
    {
        video_bitrate = "20000k";
    }
    return video_bitrate;
}