/*
 * MyIndexMapper.h
 *
 *  Created on: 9 mar 2012
 *      Author: ostenlund
 */

#ifndef MYINDEXMAPPER_H_
#define MYINDEXMAPPER_H_

#include <bb/cascades/DataModel>

class MyIndexMapper: public bb::cascades::DataModel::IndexMapper {
public:
	MyIndexMapper(int index, int count, bool deleted);
	bool newIndexPath(QVariantList *pNewIndexPath, int *pReplacementIndex,
			const QVariantList &oldIndexPath) const;

	int mIndex;
	int mCount;
	bool mDeleted;
};

#endif /* MYINDEXMAPPER_H_ */
