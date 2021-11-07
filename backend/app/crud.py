from sqlalchemy.orm import Session
from .db.models import *
from . import schemas, utils

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


"""
------------------------ Status CRUD functions ------------------------
"""
def get_status(db: Session, prototype_id: int):
	return db.query(Status).filter(Status.prototype_id == prototype_id).first()


def create_status(db: Session, prototype_id: int):
	db_status = Status(
		prototype_id=prototype_id,
		ph=0.0,
		temperature=0.0,
		humidity=0.0,
		last_update=utils.str_datetime_now())

	db.add(db_status)
	db.commit()
	db.refresh(db_status)
	return db_status

def update_status(db: Session, prototype_id: int, new_status: schemas.StatusCreate):
	# db_status = get_status(db=db, prototype_id=prototype_id)
	db_status = db.query(Status).filter(Status.prototype_id == prototype_id)
	if db_status.first() is None:
		db_status = create_status(db=db, prototype_id=prototype_id)
	else:
		db_status.update(new_status.dict())
		db.commit()
		db_status = get_status(db=db, prototype_id=prototype_id)
	
	return db_status
