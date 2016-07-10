#ifndef BDISKDOWNLOADDELEGATE_H
#define BDISKDOWNLOADDELEGATE_H

#include <QObject>
#include <QHash>
#include <QSharedDataPointer>
#include <QMutex>

#include "DLTaskAccessMgr.h"

#include "BDiskRequest/BDiskOperationRequest.h"

namespace YADownloader {
class DLTask;
class DLTaskInfo;
}

class BDiskDownloadDelegateNodePriv;
class BDiskDownloadDelegateNode
{
public:
    BDiskDownloadDelegateNode();
    BDiskDownloadDelegateNode(const BDiskDownloadDelegateNode &other);
    virtual ~BDiskDownloadDelegateNode();

    bool operator ==(const BDiskDownloadDelegateNode &other) const;
    bool operator !=(const BDiskDownloadDelegateNode &other) const;
    BDiskDownloadDelegateNode &operator =(const BDiskDownloadDelegateNode &other);

    YADownloader::DLTask *task() const;
    YADownloader::DLTaskInfo placeholderTaskInfo() const;

    QString identifier() const;

    qint64 elapsedTimeOffset() const;

    void setTask(YADownloader::DLTask *task);

    void setPlaceholderTaskInfo(const YADownloader::DLTaskInfo &info);

    void setIdentifier(const QString &hash);

    void setElapsedTimeOffset(qint64 offset);

private:
    YADownloader::DLTask *m_task;
    QSharedDataPointer<BDiskDownloadDelegateNodePriv> d;
};

class QTimer;
class BDiskEvent;
class BDiskDownloadDelegate : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QVariantList tasks READ tasks WRITE setTasks NOTIFY tasksChanged)
public:
    explicit BDiskDownloadDelegate(QObject *parent = 0);
    virtual ~BDiskDownloadDelegate();

    Q_INVOKABLE void download(const QString &from, const QString &savePath, const QString &saveName);
    Q_INVOKABLE bool taskRunning(const QString &hash);

    QVariantList tasks() const;

signals:
    void tasksChanged(const QVariantList &tasks);

public slots:
    void stop(const QString &hash);
    void resume(const QString &hash);

private slots:
    void setTasks(const QVariantList &tasks);

private:
    ///
    /// \brief parseDLTaskInfoList
    /// Parse DLTaskInfoList and insert to m_taskInfoHash
    /// \param list
    ///
    void parseDLTaskInfoList(const YADownloader::DLTaskInfoList &list);
    QVariantList convertTaskInfoHash();

private:
    BDisOpDownload m_downloadOp;
    YADownloader::DLTaskAccessMgr *m_downloadMgr;
    QTimer *m_timer;
//    BDiskEvent *m_diskEvent;
    QMutex m_locker;
    QHash<QString, BDiskDownloadDelegateNode> m_nodeHash;
    QHash<QString, BDiskDownloadDelegateNode> m_tmpNode;

    QVariantList m_tasks;
    qint64 m_timerCount;
    qint64 m_timerStartedMSecsSinceEpoch;
};

#endif // BDISKDOWNLOADDELEGATE_H
