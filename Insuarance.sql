-- phpMyAdmin SQL Dump
-- version 4.2.12deb2+deb8u1
-- http://www.phpmyadmin.net
--
-- Хост: localhost
-- Время создания: Дек 14 2015 г., 10:30
-- Версия сервера: 5.5.46-0+deb8u1
-- Версия PHP: 5.6.14-0+deb8u1

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- База данных: `Insuarance`
--
CREATE DATABASE IF NOT EXISTS `Insuarance` DEFAULT CHARACTER SET utf8 COLLATE utf8_bin;
USE `Insuarance`;

-- --------------------------------------------------------

--
-- Структура таблицы `Clients`
--

DROP TABLE IF EXISTS `Clients`;
CREATE TABLE `Clients` (
`CID` int(11) NOT NULL,
  `firstName` varchar(255) COLLATE utf8_bin NOT NULL,
  `lastName` varchar(255) COLLATE utf8_bin NOT NULL,
  `passport` varchar(20) COLLATE utf8_bin NOT NULL
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COLLATE=utf8_bin;

-- --------------------------------------------------------

--
-- Структура таблицы `InsuranceDeal`
--

DROP TABLE IF EXISTS `InsuranceDeal`;
CREATE TABLE `InsuranceDeal` (
`InsDealID` int(11) NOT NULL,
  `InsTypeID` int(11) NOT NULL,
  `CID` int(11) NOT NULL,
  `createdAt` date NOT NULL,
  `validTo` date NOT NULL,
  `cost` decimal(11,2) NOT NULL,
  `param` text COLLATE utf8_bin
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COLLATE=utf8_bin;

-- --------------------------------------------------------

--
-- Структура таблицы `InsuranceType`
--

DROP TABLE IF EXISTS `InsuranceType`;
CREATE TABLE `InsuranceType` (
`InsTypeID` int(11) NOT NULL,
  `name` varchar(255) COLLATE utf8_bin NOT NULL,
  `priceForDay` decimal(11,2) NOT NULL
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COLLATE=utf8_bin;

--
-- Индексы сохранённых таблиц
--

--
-- Индексы таблицы `Clients`
--
ALTER TABLE `Clients`
 ADD PRIMARY KEY (`CID`), ADD KEY `firstName` (`firstName`,`lastName`);

--
-- Индексы таблицы `InsuranceDeal`
--
ALTER TABLE `InsuranceDeal`
 ADD PRIMARY KEY (`InsDealID`), ADD KEY `InsTypeID` (`InsTypeID`), ADD KEY `CID` (`CID`);

--
-- Индексы таблицы `InsuranceType`
--
ALTER TABLE `InsuranceType`
 ADD PRIMARY KEY (`InsTypeID`), ADD UNIQUE KEY `name` (`name`);

--
-- AUTO_INCREMENT для сохранённых таблиц
--

--
-- AUTO_INCREMENT для таблицы `Clients`
--
ALTER TABLE `Clients`
MODIFY `CID` int(11) NOT NULL AUTO_INCREMENT,AUTO_INCREMENT=3;
--
-- AUTO_INCREMENT для таблицы `InsuranceDeal`
--
ALTER TABLE `InsuranceDeal`
MODIFY `InsDealID` int(11) NOT NULL AUTO_INCREMENT,AUTO_INCREMENT=14;
--
-- AUTO_INCREMENT для таблицы `InsuranceType`
--
ALTER TABLE `InsuranceType`
MODIFY `InsTypeID` int(11) NOT NULL AUTO_INCREMENT,AUTO_INCREMENT=6;
--
-- Ограничения внешнего ключа сохраненных таблиц
--

--
-- Ограничения внешнего ключа таблицы `InsuranceDeal`
--
ALTER TABLE `InsuranceDeal`
ADD CONSTRAINT `InsuranceDeal_ibfk_1` FOREIGN KEY (`InsTypeID`) REFERENCES `InsuranceType` (`InsTypeID`),
ADD CONSTRAINT `InsuranceDeal_ibfk_2` FOREIGN KEY (`CID`) REFERENCES `Clients` (`cid`);

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
