#include <QtWidgets>
#include <QtGui>
#include <QtCore>

int main (int argc, char **argv)
{
   QApplication app(argc, argv);
   auto mainWindow = new QMainWindow();
   mainWindow->show();

   auto scene = new QGraphicsScene(mainWindow);
   QGraphicsView view(scene, mainWindow);
   view.show();

   QFile file(QApplication::arguments()[1]);
   if (file.open (QIODevice::Text | QIODevice::ReadOnly)) {
      QTextStream stream(&file);
      QString buffer = stream.readAll();
      QRegExp rx("set\\s+output\\s+\\W*([^\\'\\\"]*)");
      if (rx.indexIn(buffer) > 0) {
	 auto filename = rx.cap(1);
	 
	 auto ret = QProcess::execute("gnuplot", QApplication::arguments().mid(1));
	 if (ret >= 0) {
	    QPixmap pix(filename);
	    auto size = pix.size();
	    size += QSize(10,10);
	    view.resize (size);
	    mainWindow->resize (size);
	    
	    QGraphicsPixmapItem *item (scene->addPixmap(pix));
	    item->setPixmap(pix);
	 }
      }
   }
   return app.exec();
}
