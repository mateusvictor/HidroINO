from datetime import datetime
from typing import List
from fastapi import Depends, FastAPI, HTTPException
from fastapi.middleware.cors import CORSMiddleware
from sqlalchemy.orm import Session
from . import crud, schemas

from .db import models
from .db.database import SessionLocal, engine

'''
uvicorn --host 0.0.0.0 main.app:app --reload
'''

models.Base.metadata.create_all(bind=engine)

app = FastAPI(
	title='HidroINO API',
	description='API designed to receive data from Arduino and serve the front-end application.'
)

origins = [
	'http://localhost',
	'http://192.168.0.17',
	'http://192.168.0.14',
	'http://127.0.0.1:54219'
]

origins = [
	'*'
]
app.add_middleware(
	CORSMiddleware,
	allow_origins=origins,
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
	return crud.create_prototype(db=db, prototype=prototype)


"""
------------------------ Record model endpoints ------------------------
"""
@app.get('/records/', response_model=List[schemas.Record], tags=['records'])
def read_records(prototype_id: int = None, db: Session = Depends(get_db)):
	db_prototype = crud.get_prototype(db=db, prototype_id=prototype_id) 
	if not db_prototype and prototype_id is not None:
		raise HTTPException(status_code=404, detail='Invalid prototype ID')
	
	db_records = crud.get_records(db=db, prototype_id=prototype_id)
	return db_records


@app.get('/record-last/{prototype_id}/', response_model=schemas.Record, tags=['records'])
def read_last_record(prototype_id: int, db: Session = Depends(get_db)):
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
