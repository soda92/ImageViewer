#include <QApplication>
#include <QDebug>
#include <QFileDialog>
#include <QLabel>
#include <QLayout>
#include <QMainWindow>
#include <QPixmap>
#include <QPushButton>
#include <QScrollArea>

class ImageViewer : public QMainWindow {
  Q_OBJECT

public:
  ImageViewer(QWidget *parent = nullptr) : QMainWindow(parent) {
    layout_main->addWidget(image_label);
    layout_buttons->addWidget(select_file_button);
    layout_main->addLayout(layout_buttons);
    main_widget->setLayout(layout_main);
    this->setCentralWidget(main_widget);

    connect(select_file_button, &QPushButton::clicked, this,
            &ImageViewer::openFile);

    connect(this, &ImageViewer::show_image_signal, this,
            &ImageViewer::showImage);
    showImage("D:/favicon.png");
  }

private:
  QWidget *main_widget{new QWidget};

  QHBoxLayout *layout_main{new QHBoxLayout};
  QVBoxLayout *layout_buttons{new QVBoxLayout};
  QImage image{};
  QLabel *image_label{new QLabel("图片区域")};

  //  QScrollArea *scroll_area{new QScrollArea};
  //  double scale_factor{1.0};

  QPushButton *select_file_button{new QPushButton("选择文件")};

signals:
  void show_image_signal(QString fileName);

private slots:
  void openFile() {
    QString fileName = QFileDialog::getOpenFileName(this, "打开文件", "D:/",
                                                    "Images (*.png *.jpg)");
    qDebug() << fileName;
    emit show_image_signal(fileName);
  }

  void showImage(QString fileName) {
    this->image.load(fileName);
    this->image_label->setPixmap(QPixmap::fromImage(this->image));
  }
};

#include "main.moc"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  ImageViewer w;
  w.show();
  return a.exec();
}
