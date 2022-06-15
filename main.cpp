#include <QApplication>
#include <QDebug>
#include <QEvent>
#include <QFileDialog>
#include <QGuiApplication>
#include <QLabel>
#include <QLayout>
#include <QMainWindow>
#include <QMenu>
#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>
#include <QPushButton>
#include <QScreen>
#include <QScrollArea>

class ImageScaleLabel : public QLabel {
  Q_OBJECT

public:
  ImageScaleLabel(QWidget *parent = nullptr)
      : ImageScaleLabel("ImageScaleLabel", parent) {}

  ImageScaleLabel(const QString &text, QWidget *parent = nullptr)
      : QLabel(text, parent) {
    this->setScaledContents(true);
  }

protected:
  void mousePressEvent(QMouseEvent *e) override final {
    if (e->button() == Qt::RightButton) {
      selection_rect_ = {};
      repaint();
      if (selection_rect_.contains(e->pos()))
        context_menu_.exec(this->mapToGlobal(e->pos()));
    } else {
      selection_started_ = true;
      selection_rect_.setTopLeft(e->pos());
      selection_rect_.setBottomRight(e->pos());
    }
  }

  void paintEvent(QPaintEvent *e) override final {
    QLabel::paintEvent(e);
    QPainter painter(this);
    painter.setPen(QPen(QBrush(QColor(0, 0, 0, 180)), 1, Qt::DashLine));
    painter.setBrush(QBrush(QColor(255, 255, 255, 120)));

    painter.drawRect(selection_rect_);
  }

  void mouseMoveEvent(QMouseEvent *e) override final {
    if (selection_started_) {
      selection_rect_.setBottomRight(e->pos());
      repaint();
    }
  }
  void mouseReleaseEvent(QMouseEvent *) override final {
    selection_started_ = false;
    //    this->setPixmap(this->pixmap()->copy(selection_rect_));
  }

private:
  bool selection_started_{false};
  QRect selection_rect_{};
  QMenu context_menu_{};
};

class ImageViewer : public QMainWindow {
  Q_OBJECT

public:
  ImageViewer(QWidget *parent = nullptr) : QMainWindow(parent) {
    showImage("D:/wallpaper-example.jpg");
    image_scroll_area_->setWidget(image_scale_label_);
    layout_main_->addWidget(image_scroll_area_);

    layout_buttons_->addWidget(select_file_button_);
    layout_buttons_->addWidget(zoom_in_button_);
    layout_buttons_->addWidget(zoom_out_button_);
    layout_buttons_->addWidget(zoom_reset_button_);
    buttons_widget_->setLayout(layout_buttons_);
    buttons_widget_->setFixedWidth(200);

    layout_main_->addWidget(buttons_widget_);
    main_widget_->setLayout(layout_main_);
    this->setCentralWidget(main_widget_);

    connect(select_file_button_, &QPushButton::clicked, this,
            &ImageViewer::openFile);

    connect(zoom_in_button_, &QPushButton::clicked, this, &ImageViewer::zoomIn);
    connect(zoom_out_button_, &QPushButton::clicked, this,
            &ImageViewer::zoomOut);
    connect(zoom_reset_button_, &QPushButton::clicked, this,
            &ImageViewer::zoomReset);

    resize(QGuiApplication::primaryScreen()->availableSize() * 3 / 5);
  }

private:
  QWidget *main_widget_{new QWidget};
  QWidget *buttons_widget_{new QWidget};

  QHBoxLayout *layout_main_{new QHBoxLayout};
  QVBoxLayout *layout_buttons_{new QVBoxLayout};
  QImage image_{};
  ImageScaleLabel *image_scale_label_{new ImageScaleLabel("图片区域")};

  QScrollArea *image_scroll_area_{new QScrollArea};
  double scale_factor_{1.0};

  QPushButton *select_file_button_{new QPushButton("选择文件")};
  QPushButton *zoom_in_button_{new QPushButton("放大")};
  QPushButton *zoom_out_button_{new QPushButton("缩小")};
  QPushButton *zoom_reset_button_{new QPushButton("重置")};

  void showImage(QString fileName) {
    image_.load(fileName);
    image_scale_label_->setPixmap(QPixmap::fromImage(this->image_));
    zoomReset();
  }

  void scaleImage(double scale_factor) {
    scale_factor_ *= scale_factor;
    qDebug() << "zoom level:" << scale_factor_;
    image_scale_label_->resize(scale_factor_ *
                               image_scale_label_->pixmap()->size());
  }

private slots:
  void openFile() {
    QString fileName = QFileDialog::getOpenFileName(this, "打开文件", "D:/",
                                                    "Images (*.png *.jpg)");
    qDebug() << "file name:" << fileName;
    if (fileName != "") {
      showImage(fileName);
    }
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
