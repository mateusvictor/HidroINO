from datetime import datetime
from typing import List
from fastapi import Depends, FastAPI, HTTPException
from fastapi.middleware.cors import CORSMiddleware
from sqlalchemy.orm import Session
from . import crud, schemas, utils

from .db import models
from .db.database import SessionLocal, engine

'''
uvicorn --host 0.0.0.0 app.main:app --reload
'''

models.Base.metadata.create_all(bind=engine)

app = FastAPI(
	title='HidroINO API',
	description='API designed to receive data from Arduino and serve the front-end application.'
)

app.add_middleware(
	CORSMiddleware,
	allow_origins=['*'],
	allow_credentials=True,
	allow_methods=['*'],
	allow_headers=['*'],
)

def get_db():
	db = SessionLocal()
	try:
		yield db
	finally:
		db.close()


"""
------------------------ Prototype model endpoints ------------------------
"""
@app.get('/prototypes/', response_model=List[schemas.Prototype], tags=['prototypes'])
def read_prototypes(db: Session = Depends(get_db)):
	return crud.get_prototypes(db=db)


@app.post('/prototypes/', response_model=schemas.Prototype, tags=['prototypes'])
def create_prototype(prototype: schemas.PrototypeCreate, db: Session = Depends(get_db)):
	db_prototype = crud.create_prototype(db=db, prototype=prototype)
	crud.create_status(db=db, prototype_id=db_prototype.id)
	return db_prototype


"""
------------------------ Record model endpoints ------------------------
"""
@app.get('/records/', response_model=List[schemas.Record], tags=['records'])
def read_records(prototype_id: str = None, db: Session = Depends(get_db)):
	db_prototype = crud.get_prototype(db=db, prototype_id=prototype_id) 
	if not db_prototype and prototype_id is not None:
		raise HTTPException(status_code=404, detail='Invalid prototype ID')
	
	db_records = crud.get_records(db=db, prototype_id=prototype_id)
	return db_records


@app.get('/record-last/{prototype_id}/', response_model=schemas.Record, tags=['records'])
def read_last_record(prototype_id: str, db: Session = Depends(get_db)):
	db_prototype = crud.get_prototype(db=db, prototype_id=prototype_id) 
	if not db_prototype:
		raise HTTPException(status_code=404, detail='Invalid prototype ID')

	db_last_record = crud.get_last_record(db=db, prototype_id=prototype_id)
	return db_last_record


@app.post('/records/', response_model=schemas.Record, tags=['records'])
def create_record(record: schemas.RecordCreate, db: Session = Depends(get_db)):
	now = datetime.now()
	record.datetime_creation = now.strftime('%d-%m-%Y %H:%M:%S')
	return crud.create_record(db=db, record=record)



"""
------------------------ Status model endpoints ------------------------
"""
@app.get('/status/{prototype_id}/', response_model=schemas.Status, tags=['status'])
def read_status(prototype_id: str, db: Session = Depends(get_db)):
	db_prototype = crud.get_prototype(db=db, prototype_id=prototype_id) 
	if not db_prototype and prototype_id is not None:
		raise HTTPException(status_code=404, detail='Invalid prototype ID')
	
	db_records = crud.get_status(db=db, prototype_id=prototype_id)
	return db_records


@app.put('/status/{prototype_id}/', response_model=schemas.Status, tags=['status'])
def update_status(prototype_id: str, status: schemas.StatusCreate, db: Session = Depends(get_db)):
	db_prototype = crud.get_prototype(db=db, prototype_id=prototype_id) 
	if not db_prototype and prototype_id is not None:
		raise HTTPException(status_code=404, detail='Invalid prototype ID')

	print('tamolok')
	status.last_update = utils.str_datetime_now()	
	db_status = crud.update_status(db=db, prototype_id=prototype_id, new_status=status)
	return db_status