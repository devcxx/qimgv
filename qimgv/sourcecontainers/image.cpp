#include "image.h"

Image::Image(QString _path)
    : mDocInfo(new DocumentInfo(_path)),
      mLoaded(false),
      mEdited(false),
      mPath(_path)
{
}

Image::Image(std::unique_ptr<DocumentInfo> _info)
    : mDocInfo(std::move(_info)),
      mLoaded(false),
      mEdited(false),
      mPath(mDocInfo->filePath())
{
}

Image::~Image() {
}

QString Image::filePath() const {
    return mPath;
}

bool Image::isLoaded() const {
    return mLoaded;
}

DocumentType Image::type() const {
    return mDocInfo->type();
}

QString Image::fileName() const {
    return mDocInfo->fileName();
}

QString Image::baseName() const {
    return mDocInfo->baseName();
}

bool Image::isEdited() const {
    return mEdited;
}

qint64 Image::fileSize() const {
    return mDocInfo->fileSize();
}

QDateTime Image::lastModified() const {
    return mDocInfo->lastModified();
}

OrderedMap<QString, QString> Image::getExifTags() {
    return mDocInfo->getExifTags();
}

OrderedMap<QString, QString> Image::getAllTags()
{
    OrderedMap<QString, QString> tags;
    tags.insert(QObject::tr("Name"), fileName());
    tags.insert(QObject::tr("Type"), mDocInfo->mimeType().name());
    tags.insert(QObject::tr("Size"), QLocale().formattedDataSize(fileSize(), 1));
    tags.insert(QObject::tr("Modified date"), lastModified().toString("yyyy-MM-dd hh:mm"));
    tags.insert(QObject::tr("Width"), QString::number(width()));
    tags.insert(QObject::tr("Height"), QString::number(height()));
    return tags;
}

