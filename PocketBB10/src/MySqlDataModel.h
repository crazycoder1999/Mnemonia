/*
 * MySqlDataModel.h
 *
 *  Created on: 29 feb 2012
 *      Author: ostenlund
 */

#ifndef MYSQLDATAMODEL_H_
#define MYSQLDATAMODEL_H_

#include <bb/cascades/DataModel>
#include <bb/data/DataAccessReply.hpp>
#include <bb/data/SqlConnection.hpp>
using namespace bb::data;
using namespace bb::cascades;

#define NOOP 0
#define SYNC 1
#define DELETE 2
#define ADD 3
#define CHECKMARK 4
class MySqlDataModel: public DataModel {
Q_OBJECT

public:
	MySqlDataModel();
	virtual ~MySqlDataModel();

	// Implementing the DataModel class
	Q_INVOKABLE
	int childCount(const QVariantList &indexPath);
	Q_INVOKABLE
	bool hasChildren(const QVariantList &indexPath);
	Q_INVOKABLE
	QVariant data(const QVariantList &indexPath);
	Q_INVOKABLE
	void insertLink(QString id,QString title,QString desc,QString lnk);
	void deleteById(QString id);

	Q_INVOKABLE
	void deleteAll();
	// Functions for loading data and removing items
	Q_INVOKABLE
	void checkLink(QString id);
	Q_INVOKABLE
	void removeItems();
	Q_INVOKABLE
	void removeItemWithId(const QVariant &,bool);
	void addItem(QString id,QString title,QString desc,QString lnk);
	// A signal that's emitted when the initial set of data has been
	// loaded into the DataModel
	Q_INVOKABLE
	void loadData(QString orderType);
	void loadData();
	Q_SIGNAL
	void initialItemsLoaded();
	// Signals and slots used for initiating the SQL request
	Q_SIGNAL
	void loadSQLData(const QString&);
	Q_SLOT
	void onLoadData(const QString& sql);

	// A slot that asynchronously receives the reply data from the
	// SQL request
public Q_SLOTS:
	void onReply(const bb::data::DataAccessReply& dar);

private:
	void removeLoadingItem();
	void appendNewList(QVariantList resultList);
	SqlWorker *m_pSqlWorker;
	SqlConnection *m_pSqlConnection;

	QVariantList modelList;
	int mState;
};

#endif /* MYSQLDATAMODEL_H_ */
