from sqlalchemy import Boolean, Column, Integer, Float, String, ForeignKey
from sqlalchemy.orm import relationship
import uuid
from .database import Base


# class Plantation(Base):
# 	__tablename__ = 'plantations'
# 	id = Column(Integer, primary_key=True, index=True)
	

class Prototype(Base):
	__tablename__ = 'prototypes'
	id = Column(String, primary_key=True, index=True, default=str(uuid.uuid4()))
	owner_name = Column(String, index=True)
	records = relationship('Record', back_populates='prototype')
	status = relationship('Status', back_populates='prototype')


class Record(Base):
	__tablename__ = 'records'
	id = Column(Integer, primary_key=True, index=True)
	prototype_id = Column(String, ForeignKey('prototypes.id'))
	ph = Column(Float, index=True)
	temperature = Column(Float, index=True)
	humidity = Column(Float, index=True)
	datetime_creation = Column(String, index=True)

	prototype = relationship('Prototype', back_populates='records')


class Status(Base):
	__tablename__ = 'status'
	id = Column(Integer, primary_key=True, index=True)
	prototype_id = Column(String, ForeignKey('prototypes.id'), unique=True)
	ph = Column(Float, index=True)
	temperature = Column(Float, index=True)
	humidity = Column(Float, index=True)
	last_update = Column(String, index=True)

	prototype = relationship('Prototype', back_populates='status')


