#include <QApplication>
#include <QDebug>
#include <QFileDialog>
#include <QGuiApplication>
#include <QLabel>
#include <QLayout>
#include <QMainWindow>
#include <QPixmap>
#include <QPushButton>
#include <QScreen>
#include <QScrollArea>

class ImageViewer : public QMainWindow {
  Q_OBJECT

public:
  ImageViewer(QWidget *parent = nullptr) : QMainWindow(parent) {
    image_label_->setScaledContents(true);

    layout_main_->addWidget(image_label_);

    layout_buttons_->addWidget(select_file_button_);
    layout_buttons_->addWidget(zoom_in_button_);
    layout_buttons_->addWidget(zoom_out_button_);
    layout_buttons_->addWidget(zoom_reset_button_);

    layout_main_->addLayout(layout_buttons_);
    main_widget_->setLayout(layout_main_);
    this->setCentralWidget(main_widget_);

    connect(select_file_button_, &QPushButton::clicked, this,
            &ImageViewer::openFile);

    connect(zoom_in_button_, &QPushButton::clicked, this, &ImageViewer::zoomIn);
    connect(zoom_out_button_, &QPushButton::clicked, this,
            &ImageViewer::zoomOut);
    connect(zoom_reset_button_, &QPushButton::clicked, this,
            &ImageViewer::zoomReset);

    showImage("D:/wallpaper-example.jpg");
    resize(QGuiApplication::primaryScreen()->availableSize() * 3 / 5);
  }

private:
  QWidget *main_widget_{new QWidget};

  QHBoxLayout *layout_main_{new QHBoxLayout};
  QVBoxLayout *layout_buttons_{new QVBoxLayout};
  QImage image_{};
  QLabel *image_label_{new QLabel("图片区域")};

  //  QScrollArea *scroll_area{new QScrollArea};
  double scale_factor_{1.0};

  QPushButton *select_file_button_{new QPushButton("选择文件")};
  QPushButton *zoom_in_button_{new QPushButton("放大")};
  QPushButton *zoom_out_button_{new QPushButton("缩小")};
  QPushButton *zoom_reset_button_{new QPushButton("重置")};

  void showImage(QString fileName) {
    this->image_.load(fileName);
    this->image_label_->setPixmap(QPixmap::fromImage(this->image_));
  }

  void scaleImage(double scale_factor) {
    scale_factor_ *= scale_factor;
    image_label_->resize(scale_factor_ * image_label_->pixmap()->size());
  }

private slots:
  void openFile() {
    QString fileName = QFileDialog::getOpenFileName(this, "打开文件", "D:/",
                                                    "Images (*.png *.jpg)");
    qDebug() << fileName;
    showImage(fileName);
  }

  void zoomIn() { scaleImage(1.25); }

  void zoomOut() { scaleImage(0.8); }

  void zoomReset() { scaleImage(1 / scale_factor_); }
};

#include "main.moc"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  ImageViewer w;
  w.show();
  return a.exec();
}
