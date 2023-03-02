-- -----------------------------------------------------
-- Schema BrookJobPortal
-- -----------------------------------------------------
CREATE SCHEMA IF NOT EXISTS `BrookJobPortal` DEFAULT CHARACTER SET utf8 ;
USE `BrookJobPortal` ;

-- -----------------------------------------------------
-- Table `BrookJobPortal`.`skills`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `BrookJobPortal`.`skills` (
  `idskills` INT NOT NULL AUTO_INCREMENT,
  `name` VARCHAR(45) NOT NULL,
  `active` TINYINT NOT NULL,
  
  PRIMARY KEY (`idskills`),
  UNIQUE INDEX `idskills_UNIQUE` (`idskills` ASC));

-- -----------------------------------------------------
-- Table `BrookJobPortal`.`applicant`
-- -----------------------------------------------------
-- CREATE TABLE IF NOT EXISTS `BrookJobPortal`.`applicant` (
--  `idapplicant` INT NOT NULL AUTO_INCREMENT,
--  `name` VARCHAR(80) NOT NULL,
--  `active` TINYINT NULL,
--  PRIMARY KEY (`idapplicant`));
  
-- -----------------------------------------------------
-- Table `BrookJobPortal`.`company`
-- -----------------------------------------------------
-- CREATE TABLE IF NOT EXISTS `BrookJobPortal`.`company` (
--  `idacompany` INT NOT NULL AUTO_INCREMENT,
--  `name` VARCHAR(80) NOT NULL,
--  `active` TINYINT NULL,
--  PRIMARY KEY (`idcompany`));
  

-- -----------------------------------------------------
-- Table `BrookJobPortal`.`login`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `BrookJobPortal`.`login` (
  `name` VARCHAR(45) NOT NULL,
  `email` VARCHAR(45) NOT NULL,
  `password` VARCHAR(65) NOT NULL,
  `fullname` VARCHAR(50) NOT NULL,
  `active` TINYINT NOT NULL,
  'company_id' TINYINT  NULL,
  'applicant_id' TINYINT NULL,
  'user_type' enum ('admin','applicant','company') NOT NUll,
  PRIMARY KEY (`name`),
  UNIQUE INDEX `email_UNIQUE` (`email` ASC),
  UNIQUE INDEX `name_UNIQUE` (`name` ASC));

-- -----------------------------------------------------
-- Table `BrookJobPortal`.`applicant_has_skills`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `BrookJobPortal`.`applicant_has_skills` (
  `login_name` INT NOT NULL,
  `skills_idskills` INT NOT NULL,
  `status` ENUM('approved','rejected','pending')  NOT NULL DEFAULT 'pending',
  PRIMARY KEY (`applicant_idapplicant`, `skills_idskills`),
  INDEX `fk_applicant_has_skills_skills1_idx` (`skills_idskills` ASC),
  INDEX `fk_applicant_has_skills_login1_idx` (`login_name` ASC),
  CONSTRAINT `fk_applicant_has_skills_login1`
    FOREIGN KEY (`login_name`)
    REFERENCES `BrookJobPortal`.`login` (`name`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_applicant_has_skills_skills1`
    FOREIGN KEY (`skills_idskills`)
    REFERENCES `BrookJobPortal`.`skills` (`idskills`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION);

-- -----------------------------------------------------
-- Table `BrookJobPortal`.`companies_lookfor_skills`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `BrookJobPortal`.`companies_lookfor_skills` (
  `skills_idskills` INT NOT NULL,
  `login_name` INT NOT NULL,
  `status` ENUM('approved','rejected','pending')  NOT NULL DEFAULT 'pending',
  PRIMARY KEY (`skills_idskills`, `login_name`),
  INDEX `fk_companies_lookfor_skills_skills1_idx` (`skills_idskills` ASC),
  INDEX `fk_companies_lookfor_skills_login1_idx` (`login_name` ASC),
  CONSTRAINT `fk_companies_lookfor_skills_skills1`
    FOREIGN KEY (`skills_idskills`)
    REFERENCES `BrookJobPortal`.`skills` (`idskills`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_companies_lookfor_skills_login1`
    FOREIGN KEY (`login_name`)
    REFERENCES `BrookJobPortal`.`login` (`name`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION);

-- -----------------------------------------------------
-- Table `BrookJobPortal`.`Sessions`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `BrookJobPortal`.`Sessions` (
  `SessionID` VARCHAR(65) NOT NULL,
  `fk_user` VARCHAR(45) NOT NULL,
  `expires` DATETIME NOT NULL,
  `IP` VARCHAR(15) NOT NULL,
  `active` TINYINT NOT NULL,
  PRIMARY KEY (`SessionID`),
  UNIQUE INDEX `SessionID_UNIQUE` (`SessionID` ASC),
  INDEX `name_idx` (`fk_user` ASC),
  CONSTRAINT `name`
    FOREIGN KEY (`fk_user`)
    REFERENCES `BrookJobPortal`.`login` (`name`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION);

-- -----------------------------------------------------
-- Table `BrookJobPortal`.`TOTP`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `BrookJobPortal`.`TOTP` (
  `seed` VARCHAR(56) NOT NULL,
  `fk_user` VARCHAR(45) NOT NULL,
  `active` TINYINT NOT NULL,
  PRIMARY KEY (`fk_user`),
  UNIQUE INDEX `seed_UNIQUE` (`seed` ASC),
  CONSTRAINT `fk_user`
    FOREIGN KEY (`fk_user`)
    REFERENCES `BrookJobPortal`.`login` (`name`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION);

-- -----------------------------------------------------
-- Table `BrookJobPortal`.`2FA`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `BrookJobPortal`.`2FA` (
  `code` INT NOT NULL,
  `fk_user` VARCHAR(45) NOT NULL,
  `id` INT NOT NULL AUTO_INCREMENT,
  `expires` DATETIME NULL,
  INDEX `fk_user_idx` (`fk_user` ASC),
  PRIMARY KEY (`id`),
  CONSTRAINT `fk_user_2fa`
    FOREIGN KEY (`fk_user`)
    REFERENCES `BrookJobPortal`.`login` (`name`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION);

INSERT INTO login(name, password, active) VALUES("admin","root","0");

CREATE USER 'user'@'localhost' IDENTIFIED BY 'd9pifetoyesad2cekipoyolis';
GRANT SELECT ON BrookJobPortal.companies_lookfor_skills TO 'user'@'localhost'; -- % or _
GRANT SELECT ON BrookJobPortal.applicant_has_skills TO 'user'@'localhost';
GRANT SELECT ON BrookJobPortal.applicant TO 'user'@'localhost';
GRANT SELECT ON BrookJobPortal.skills TO 'user'@'localhost';

CREATE USER 'admin'@'localhost' IDENTIFIED BY 'nocikocofot9com4cif1boq6t';
GRANT SELECT ON BrookJobPortal.companies_lookfor_skills TO 'admin'@'localhost';
GRANT SELECT ON BrookJobPortal.applicant_has_skills TO 'admin'@'localhost';
GRANT INSERT ON BrookJobPortal.companies_lookfor_skills TO 'admin'@'localhost';
GRANT INSERT ON BrookJobPortal.applicant_has_skills TO 'admin'@'localhost';
GRANT DELETE ON BrookJobPortal.companies_lookfor_skills TO 'admin'@'localhost';
GRANT DELETE ON BrookJobPortal.applicant_has_skills TO 'admin'@'localhost';
GRANT SELECT ON BrookJobPortal.applicant TO 'admin'@'localhost';
GRANT SELECT ON BrookJobPortal.company TO 'admin'@'localhost';
GRANT SELECT ON BrookJobPortal.skills TO 'admin'@'localhost';
GRANT SELECT(name,fullname,active) ON BrookJobPortal.login TO 'admin'@'localhost';
GRANT INSERT ON BrookJobPortal.applicant TO 'admin'@'localhost';
GRANT UPDATE(active) ON BrookJobPortal.applicant TO 'admin'@'localhost';
GRANT INSERT ON BrookJobPortal.company TO 'admin'@'localhost';
GRANT UPDATE(active) ON BrookJobPortal.company TO 'admin'@'localhost';
GRANT INSERT ON BrookJobPortal.skills TO 'admin'@'localhost';
GRANT UPDATE(active) ON BrookJobPortal.skills TO 'admin'@'localhost';

CREATE USER 'login'@'localhost' IDENTIFIED BY 'n9ras1yay4aey8yoterec2vex';
GRANT SELECT ON BrookJobPortal.TOTP TO 'login'@'localhost';
GRANT SELECT ON BrookJobPortal.2FA TO 'login'@'localhost';
GRANT INSERT ON BrookJobPortal.2FA TO 'login'@'localhost';
GRANT SELECT ON BrookJobPortal.login TO 'login'@'localhost';
GRANT SELECT ON BrookJobPortal.Sessions TO 'login'@'localhost';
GRANT INSERT ON BrookJobPortal.Sessions TO 'login'@'localhost';
GRANT UPDATE(active) ON BrookJobPortal.Sessions TO 'login'@'localhost';

CREATE USER 'stateful'@'localhost' IDENTIFIED BY 'dajuyihobumasopin7qefiroa';
GRANT SELECT ON BrookJobPortal.Sessions TO 'stateful'@'localhost';

CREATE USER 'register'@'localhost' IDENTIFIED BY 'qaciy0t2kif5cul5d5quwanus';
GRANT SELECT ON BrookJobPortal.login TO 'register'@'localhost';
GRANT INSERT ON BrookJobPortal.login TO 'register'@'localhost';
GRANT UPDATE ON BrookJobPortal.login TO 'register'@'localhost';
GRANT UPDATE(active) ON BrookJobPortal.login TO 'register'@'localhost';
GRANT SELECT ON BrookJobPortal.TOTP TO 'register'@'localhost';
GRANT INSERT ON BrookJobPortal.TOTP TO 'register'@'localhost';
GRANT UPDATE(active,seed) ON BrookJobPortal.TOTP TO 'register'@'localhost';
GRANT SELECT ON BrookJobPortal.2FA TO 'register'@'localhost';
GRANT INSERT ON BrookJobPortal.2FA TO 'register'@'localhost';