#include "edit_core.h"
#include "core_fs_check.h"
#include "core_system.h"
#include <QFileDialog>
#include <QPixmap>
#include <QProcess>
#include <QDebug>

#include "settings.h"

extern Settings *settings;

EditCore::EditCore()
    : __penSize(2)
    , __borderWeight(2)
    , __eraseSize(10)
    , __dotSize(8)
    , __ProxyType(false)
{
    __fileFormat = "yyyy_MM_dd_hh_mm_ss";
    __saveDir = QStandardPaths::standardLocations(QStandardPaths::DesktopLocation).first();

    // init UI color
    __borderColor = QColor("#c3c3c3");
    __widgetBgColor = QColor("#ffffff");
    __normalColor = Qt::transparent;
    __hoverColor = QColor("#b6b6b6");
    __imageType = "PNG";
    __dotColor = QColor("#c3c3c3");
    __imageQuality = 100;
    __shadowEffect = false;
    __enableUpload = false;
    __enableProxy = false;
    __readOnly = false;
    __logoPixmap = QPixmap(svgImagePath() + "logo.svg");
    resetColor();
}

EditCore::~EditCore() { }

void EditCore::resetColor()
{
    __bgColor = __penColor = Qt::red;
}

void EditCore::setKeymap(const QStringList& keymap)
{
    __keymap = keymap;
}

const QStringList& EditCore::getKeymap()
{
    return __keymap;
}

bool EditCore::getStartBoot() const
{
    return __startOnBoot;
}

void EditCore::setStartBoot(bool is)
{
    __startOnBoot = is;
}

bool EditCore::getShadowEffect() const
{
    return __shadowEffect;
}

void EditCore::setShadowEffect(bool is)
{
    __shadowEffect = is;
}

const QString& EditCore::getFontFamily() const
{
    return __fontFamily;
}

void EditCore::setFontFamily(const QString& fontFamily)
{
    __fontFamily = fontFamily;
}

bool EditCore::getLanguage() const
{
    return __language;
}

void EditCore::setLanguage(bool lang)
{
    __language = lang;
}

QSystemTrayIcon* EditCore::getSysTray()
{
    return __sysTray;
}

void EditCore::setSysTray(QSystemTrayIcon* systemTray)
{
    __sysTray = systemTray;
}

int EditCore::getDotSize() const
{
    return __dotSize;
}

void EditCore::setDotSize(int s)
{
    __dotSize = s;
}

QColor EditCore::getDotColor() const
{
    return settings->colorScheme().widget_border;
//    return __dotColor;
}

void EditCore::setDotColor(QColor c)
{
    __dotColor = c;
}

const QPixmap& EditCore::logoPixmap() const
{
    return __logoPixmap;
}

void EditCore::setEnableUpload(bool is)
{
    __enableUpload = is;
}

bool EditCore::getEnableUpload()
{
    return __enableUpload;
}

void EditCore::setEnableProxy(bool is)
{
    __enableProxy = is;
}

bool EditCore::getEnableProxy()
{
    return __enableProxy;
}

void EditCore::setProxyType(bool is)
{
    __ProxyType = is;
}

bool EditCore::getProxyType()
{
    return __ProxyType;
}

void EditCore::setProxyIP(const QString& ip)
{
    __IP = ip;
}

const QString& EditCore::getProxyIP()
{
    return __IP;
}

void EditCore::setProxyPort(int port)
{
    __Port = port;
}

int EditCore::getProxyPort()
{
    return __Port;
}

void EditCore::setSMMSAuthorization(const QString& auth)
{
    __Authorization = auth;
}

const QString& EditCore::getSMMSAuthorization()
{
    return __Authorization;
}

void EditCore::setImageReadonly(bool readonly)
{
    __readOnly = readonly;
}

bool EditCore::imageReadonly()
{
    return __readOnly;
}

void EditCore::setWritePath(const QString& path)
{
    __writePath = path;
}

QString EditCore::writePath()
{
    return __writePath;
}

void EditCore::grabScreen(int x, int y, int w, int h)
{
    setPixMap(getPrimaryScreen()->grabWindow(0, x, y, w, h));
}

void EditCore::grabScreen(const QRect& region)
{
    grabScreen(region.left(), region.top(), region.width(), region.height());
}

QString EditCore::grabScreen2File(int x, int y, int w, int h)
{
    grabScreen(x, y, w, h);
    return PixMap2ImageFile(__pixmap);
}

void EditCore::grabScreen2File(const QRect& region)
{
    grabScreen2File(region.left(), region.top(), region.width(), region.height());
}

void EditCore::grabScreen2Clipboard(int x, int y, int w, int h)
{
    grabScreen(x, y, w, h);
    PixMap2ClipBoard(__pixmap);
}

void EditCore::grabScreen2Clipboard(const QRect& region)
{
    grabScreen2Clipboard(region.left(), region.top(), region.width(), region.height());
}

void EditCore::setBgPixmap(const QPixmap& bg)
{
    __bgPixmap = bg;
}

void EditCore::setPixMap(const QPixmap& pixmap)
{
    __pixmap = pixmap;
}

QPixmap& EditCore::getPixMap()
{
    return __pixmap;
}

QPixmap& EditCore::getbgPixmap()
{
    return __bgPixmap;
}

QString EditCore::PixMap2ImageFile(const QPixmap& pixmap)
{
    QString filenameSave; 
    if (__readOnly) {
        QString dts = QDateTime::currentDateTime().toString(__fileFormat);
        QString filename = __saveDir + "/" + prefix() + dts + "." + (__imageType.toLower());
        core_fs_check fsc(filename, getSaveFileDir(), this);

        filename = QDir::toNativeSeparators(filename);
        filenameSave = QFileDialog::getSaveFileName(nullptr, tr("Save File"),
            filename,
            "Images (*.png *.xpm *.jpg *.bmp *.jpeg)");

        if (filenameSave.isEmpty())
            return filenameSave;
    } else {
        filenameSave = writePath();
    }
    QFileInfo fileinfo(filenameSave);

    QImageWriter iw(filenameSave);
    iw.setFormat(fileinfo.suffix().toUtf8());

    if (__imageQuality <= 0)
        __imageQuality = 1;
    else if (__imageQuality >= 100)
        __imageQuality = 100;
    iw.setQuality(__imageQuality);
    iw.write(pixmap.toImage());

    return filenameSave;
}

void EditCore::PixMap2ClipBoard(const QPixmap& pixmap)
{
    QClipboard* clipBoard = QGuiApplication::clipboard();
    clipBoard->setPixmap(pixmap);
}

void EditCore::setFilePrefixFormat(const QString& format)
{
    __fileFormat = format;
}

const QString& EditCore::getFilePrefixFormat() const
{
    return __fileFormat;
}

QScreen* EditCore::getPrimaryScreen()
{
    return QGuiApplication::primaryScreen();
}

QSize EditCore::getScreenSize()
{
    return getPrimaryScreen()->size();
}

QStringList EditCore::getCurrentUserPaths()
{
    QStringList sl;
    sl.push_back(QStandardPaths::standardLocations(QStandardPaths::HomeLocation).first());
    sl.push_back(QStandardPaths::standardLocations(QStandardPaths::DesktopLocation).first());
    sl.push_back(QStandardPaths::standardLocations(QStandardPaths::PicturesLocation).first());
    sl.push_back(QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).first());
    sl.push_back(QStandardPaths::standardLocations(QStandardPaths::MusicLocation).first());
    sl.push_back(QStandardPaths::standardLocations(QStandardPaths::MoviesLocation).first());
    return sl;
}

QStringList EditCore::getSupportedImageFormats()
{
    QStringList sl;
    for (auto x : QImageWriter::supportedImageFormats())
        sl.push_back(x.toUpper());
    return sl;
}

void EditCore::showInFolder(const QString& filePath)
{
    const QFileInfo fileInfo(filePath);
#ifdef Q_OS_WIN
    const QString explorer = "explorer.exe";
    if (explorer.isEmpty()) {
        //qWarning() << tr("Launching Windows Explorer Failed");
        return;
    }
    QStringList param;
    if (!fileInfo.isDir())
        param += QLatin1String("/select,");
    param += QDir::toNativeSeparators(fileInfo.canonicalFilePath());
    QProcess::startDetached(explorer, param);
#endif
#ifdef Q_OS_MAC
    QStringList scriptArgs;
    scriptArgs << QLatin1String("-e")
               << QString::fromLatin1("tell application \"Finder\" to reveal POSIX file \"%1\"")
                      .arg(fileInfo.canonicalFilePath());
    QProcess::execute(QLatin1String("/usr/bin/osascript"), scriptArgs);
    scriptArgs.clear();
    scriptArgs << QLatin1String("-e")
               << QLatin1String("tell application \"Finder\" to activate");
    QProcess::execute(QLatin1String("/usr/bin/osascript"), scriptArgs);
#endif
}

void EditCore::setSaveFileDir(const QString& dir)
{
    __saveDir = dir;
}

const QString& EditCore::getSaveFileDir() const
{
    return __saveDir;
}

int EditCore::getPenSize() const
{
    return __penSize;
}

void EditCore::setPenSize(int penSize)
{
    __penSize = penSize;
}

int EditCore::getBorderWeight() const
{
    return __borderWeight;
}

void EditCore::setBorderWeight(int borderWeight)
{
    __borderWeight = borderWeight;
}

int EditCore::getEraseSize() const
{
    return __eraseSize;
}

void EditCore::setEraseSize(int eraseSize)
{
    __eraseSize = eraseSize;
}

int EditCore::getImageQuality() const
{
    return __imageQuality;
}

void EditCore::setImageQuality(int imageQuality)
{
    __imageQuality = imageQuality;
}

const QString& EditCore::getImageType() const
{
    return __imageType;
}

void EditCore::setImageType(const QString& imageType)
{
    __imageType = imageType;
}

QFont& EditCore::getFont()
{
    return __font;
}

void EditCore::setFont(const QFont& font)
{
    __font = font;
}

QColor EditCore::getBorderColor() const
{
    return settings->colorScheme().widget_border;
//    return __borderColor;
}

void EditCore::setBorderColor(const QColor& color)
{
    __borderColor = color;
}

QColor EditCore::getPenColor() const
{
    return __penColor;
}

void EditCore::setPenColor(const QColor& color)
{
    __penColor = color;
}

QColor EditCore::getBgColor() const
{
    return settings->colorScheme().background;
//    return __bgColor;
}

void EditCore::setBgColor(const QColor& bgcolor)
{
    __bgColor = bgcolor;
}

QColor EditCore::getWidgetBgColor() const
{
    return settings->colorScheme().widget;
//    return __widgetBgColor;
}

void EditCore::setWidgetBgColor(const QColor& color)
{
    __widgetBgColor = __hoverColor = color;
}

QColor EditCore::getNormalColor() const
{
//    return settings->colorScheme().icons;
    return __normalColor;
}

void EditCore::setNormalColor(const QColor& c)
{
    __normalColor = c;
}

QColor EditCore::getHoverColor() const
{
    return __hoverColor;
}

void EditCore::setHoverColor(const QColor& c)
{
    __widgetBgColor = __hoverColor = c;
}

QColor EditCore::getPressedColor() const
{
    return __hoverColor.darker(120);
}

QColor EditCore::getCheckedColor() const
{
    return __hoverColor.darker(140);
}

ShapeType EditCore::getSingleShape() const
{
    return __shapeType;
}

void EditCore::setSingleShape(ShapeType st)
{
    __shapeType = st;
}

QStringList EditCore::getFontFamilies()
{
    return QFontDatabase().families();
}

QFont EditCore::getAppFont()
{
    return QApplication::font();
}
