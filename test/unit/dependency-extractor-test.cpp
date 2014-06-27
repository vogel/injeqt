/*
 * %injeqt copyright begin%
 * Copyright 2014 Rafał Malinowski (rafal.przemyslaw.malinowski@gmail.com)
 * %injeqt copyright end%
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "dependency-extractor.h"
#include "dependency-extractor.cpp"

#include <QtCore/QMetaType>
#include <QtTest/QtTest>

class InjectableType1 : public QObject
{
	Q_OBJECT
};
Q_DECLARE_METATYPE(InjectableType1 *);

class InjectableType2 : public QObject
{
	Q_OBJECT
};
Q_DECLARE_METATYPE(InjectableType2 *)

class InjectableType3 : public QObject
{
	Q_OBJECT
};
Q_DECLARE_METATYPE(InjectableType3 *)

class NonInjectableType1 : public QObject
{
	Q_OBJECT
};

class NonInjectableType2
{
};

class ValidInjectedType : public QObject
{
	Q_OBJECT

public slots:
	injeqt_setter void injeqtSetter1(InjectableType1 *) {}
	injeqt_setter void injeqtSetter2(InjectableType2 *) {}
	void noninjeqtSetter1(InjectableType1 *) {}
	void noninjeqtSetter2(InjectableType2 *) {}
	void noninjeqtSetter3(NonInjectableType1 *) {}
	void noninjeqtSetter4(NonInjectableType2 *) {}
	void noninjeqtSetter5(int) {}

};

class InheritingValidInjectedType : public ValidInjectedType
{
	Q_OBJECT

public slots:
	injeqt_setter void injeqtSetter3(InjectableType3 *) {}

};


class test_dependency_extractor : public QObject
{
	Q_OBJECT

private slots:
	void shouldFindAllValidDependencies();
	void shouldFindAllValidDependenciesInHierarchy();

};

void test_dependency_extractor::shouldFindAllValidDependencies()
{
	auto dependencies = injeqt::details::dependency_extractor{}.extract_dependencies(ValidInjectedType::staticMetaObject);
	QCOMPARE(dependencies.size(), 2UL);
	QVERIFY(dependencies.find(&InjectableType1::staticMetaObject) != std::end(dependencies));
	QVERIFY(dependencies.find(&InjectableType2::staticMetaObject) != std::end(dependencies));
}

void test_dependency_extractor::shouldFindAllValidDependenciesInHierarchy()
{
	auto dependencies = injeqt::details::dependency_extractor{}.extract_dependencies(InheritingValidInjectedType::staticMetaObject);
	QCOMPARE(dependencies.size(), 3UL);
	QVERIFY(dependencies.find(&InjectableType1::staticMetaObject) != std::end(dependencies));
	QVERIFY(dependencies.find(&InjectableType2::staticMetaObject) != std::end(dependencies));
	QVERIFY(dependencies.find(&InjectableType3::staticMetaObject) != std::end(dependencies));
}

QTEST_APPLESS_MAIN(test_dependency_extractor);

#include "dependency-extractor-test.moc"
