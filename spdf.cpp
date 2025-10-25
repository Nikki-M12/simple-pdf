#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QFileDialog>
#include <QMessageBox>
#include <QScrollArea>
#include <QKeyEvent>
#include <QWidget>
#include <QPixmap>
#include <QScrollBar>
#include <poppler-qt5.h>

class PDFViewer : public QMainWindow {
    Q_OBJECT
public:
    PDFViewer(QWidget *parent = nullptr) : QMainWindow(parent) {
        setupUI();
        connectSignals();
        setFocusPolicy(Qt::StrongFocus);
    }
    ~PDFViewer() {
        if (pdfDocument) {
            delete pdfDocument;
        }
    }
private slots:
    void openPDF() {
        QString filePath = QFileDialog::getOpenFileName(
            this,
            "Select PDF",
            "",
            "PDF files (*.pdf)"
        );
        if (!filePath.isEmpty()) {
            try {
                if (pdfDocument) {
                    delete pdfDocument;
                    pdfDocument = nullptr;
                }
                pdfDocument = Poppler::Document::load(filePath);
                if (!pdfDocument || pdfDocument->isLocked()) {
                    QMessageBox::critical(this, "Error", "Error opening PDF");
                    return;
                }
                totalPages = pdfDocument->numPages();
                currentPage = 0;
                zoomLevel = 1.0;
                updatePageInfo();
                displayPage();
            } catch (const std::exception& e) {
                QMessageBox::critical(this, "Error", QString("Error opening PDF: %1").arg(e.what()));
            }
        }
    }
    void nextPage() {
        if (pdfDocument && currentPage < totalPages - 1) {
            currentPage++;
            updatePageInfo();
            displayPage();
        }
    }
    void previousPage() {
        if (pdfDocument && currentPage > 0) {
            currentPage--;
            updatePageInfo();
            displayPage();
        }
    }
    void zoomIn() {
        changeZoom(1.2);
    }
    void zoomOut() {
        changeZoom(0.8);
    }
private:
    void setupUI() {
        setWindowTitle("SimplePDF");
        resize(1000, 700);
        QWidget *centralWidget = new QWidget(this);
        setCentralWidget(centralWidget);
        QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
        mainLayout->setContentsMargins(10, 10, 10, 10);
        QHBoxLayout *controlLayout = new QHBoxLayout();
        QPushButton *openButton = new QPushButton("Open PDF", this);
        QPushButton *prevButton = new QPushButton("◀ Previous", this);
        QPushButton *nextButton = new QPushButton("Next ▶", this);
        QPushButton *zoomInButton = new QPushButton("Zoom +", this);
        QPushButton *zoomOutButton = new QPushButton("Zoom -", this);
        pageInfo = new QLabel("Page: 0/0", this);
        controlLayout->addWidget(openButton);
        controlLayout->addWidget(prevButton);
        controlLayout->addWidget(nextButton);
        controlLayout->addWidget(zoomInButton);
        controlLayout->addWidget(zoomOutButton);
        controlLayout->addWidget(pageInfo);
        controlLayout->addStretch();
        scrollArea = new QScrollArea(this);
        scrollArea->setBackgroundRole(QPalette::Dark);
        scrollArea->setWidgetResizable(true);
        imageLabel = new QLabel(this);
        imageLabel->setBackgroundRole(QPalette::Base);
        imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
        imageLabel->setScaledContents(false);
        imageLabel->setAlignment(Qt::AlignCenter);
        scrollArea->setWidget(imageLabel);
        mainLayout->addLayout(controlLayout);
        mainLayout->addWidget(scrollArea);
        connect(openButton, &QPushButton::clicked, this, &PDFViewer::openPDF);
        connect(prevButton, &QPushButton::clicked, this, &PDFViewer::previousPage);
        connect(nextButton, &QPushButton::clicked, this, &PDFViewer::nextPage);
        connect(zoomInButton, &QPushButton::clicked, this, &PDFViewer::zoomIn);
        connect(zoomOutButton, &QPushButton::clicked, this, &PDFViewer::zoomOut);
    }
    void connectSignals() {
    }
    void keyPressEvent(QKeyEvent *event) override {
        switch (event->key()) {
            case Qt::Key_Left:
                previousPage();
                break;
            case Qt::Key_Right:
                nextPage();
                break;
            case Qt::Key_Plus:
            case Qt::Key_Equal:
                zoomIn();
                break;
            case Qt::Key_Minus:
                zoomOut();
                break;
            default:
                QMainWindow::keyPressEvent(event);
        }
    }
    void displayPage() {
        if (!pdfDocument) return;
        try {
            Poppler::Page *page = pdfDocument->page(currentPage);
            if (!page) {
                QMessageBox::critical(this, "Error", "Error loading page");
                return;
            }
            double dpi = 72.0 * zoomLevel;
            QImage image = page->renderToImage(dpi, dpi);
            delete page;
            if (image.isNull()) {
                QMessageBox::critical(this, "Error", "Error rendering page");
                return;
            }
            currentPixmap = QPixmap::fromImage(image);
            imageLabel->setPixmap(currentPixmap);
            imageLabel->resize(currentPixmap.size());
        } catch (const std::exception& e) {
            QMessageBox::critical(this, "Error", QString("Error displaying page: %1").arg(e.what()));
        }
    }
    void changeZoom(double factor) {
        zoomLevel *= factor;
        if (pdfDocument) {
            displayPage();
        }
    }
    void updatePageInfo() {
        pageInfo->setText(QString("Page: %1/%2").arg(currentPage + 1).arg(totalPages));
    }
    Poppler::Document *pdfDocument = nullptr;
    int currentPage = 0;
    int totalPages = 0;
    double zoomLevel = 1.0;
    QLabel *pageInfo = nullptr;
    QScrollArea *scrollArea = nullptr;
    QLabel *imageLabel = nullptr;
    QPixmap currentPixmap;
};
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    PDFViewer viewer;
    viewer.show();
    return app.exec();
}
#include "spdf.moc"
