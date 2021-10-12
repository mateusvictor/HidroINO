from sqlalchemy.orm import Session
from .db.models import *
from . import schemas

"""
------------------------ Prototype CRUD functions ------------------------
"""
def get_prototypes(db: Session):
	return db.query(Prototype).all()


def get_prototype(db: Session, prototype_id: int):
	return db.query(Prototype).filter(Prototype.id == prototype_id).first()


def create_prototype(db: Session, prototype: schemas.PrototypeCreate):
	db_prototype = Prototype(**prototype.dict())
	db.add(db_prototype)
	db.commit()
	db.refresh(db_prototype)
	return db_prototype


"""
------------------------ Record CRUD functions ------------------------
"""
def get_records(db: Session, prototype_id: int = None):
	if prototype_id is None:
		return db.query(Record).all()
	return db.query(Record).filter(Record.prototype_id == prototype_id).all()
	

def get_last_record(db: Session, prototype_id: int):
	db_last_record = db.query(Record).filter(Record.prototype_id == prototype_id
		).order_by(Record.id.desc()).first()

	return db_last_record

def create_record(db: Session, record: schemas.RecordCreate):
	db_record = Record(**record.dict())
	db.add(db_record)
	db.commit()
	db.refresh(db_record)
	return db_record
