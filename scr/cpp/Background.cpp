#include "../h/Background.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>

ZoomableImageViewer::ZoomableImageViewer(const QString& imgPath)
{
    // ─── 加载图片 ───
    // Python 中用 os.path.exists + os.path.abspath 做了路径校验，
    // C++ 中用 QFileInfo 一次完成这两个判断。
    if (const QFileInfo fi(imgPath); fi.exists()) {
        m_originalImage = QImage(fi.absoluteFilePath());
    }

    // 加载失败（文件不存在 / 格式无法识别）时，创建占位图
    if (m_originalImage.isNull()) {
        m_originalImage = QImage(200, 150, QImage::Format_RGB32);
        m_originalImage.fill(Qt::white);   // Qt::GlobalColor 隐式转 QColor
    }

    // 统一转为 RGBA8888（可选，QPainter 本身支持多种格式）
    m_originalImage = m_originalImage.convertToFormat(QImage::Format_RGBA8888);
    m_imgW = m_originalImage.width();
    m_imgH = m_originalImage.height();

    // ─── 读取配置文件 /../data/background.json ───
    // 注意：与 Python 一样使用“相对于当前工作目录”的路径。
    // 若程序从其他目录启动，请改用 QDir(QCoreApplication::applicationDirPath())
    // 或 QStandardPaths 定位实际配置文件位置。
    const QString configPath = QDir::currentPath() + "/../data/background.json";

    if (QFile file(configPath); file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        const QByteArray bytes = file.readAll();
        file.close();

        QJsonParseError parseError{};

        if (const QJsonDocument doc = QJsonDocument::fromJson(bytes, &parseError);
            parseError.error == QJsonParseError::NoError && doc.isObject()) {
            m_backgroundSettings = doc.object();
        }
    }

    // 安全取值：键缺失或类型不对时回退到默认值，避免崩溃
    m_maxScale = m_backgroundSettings.value("max_scale").toDouble(500.0);

    // ─── 变换状态 ───
    // m_scale / m_targetScale / m_lastScale / m_posX / m_posY 等
    // 已在头文件中用类内默认值初始化（见 background.h），此处无需重复赋值。
}