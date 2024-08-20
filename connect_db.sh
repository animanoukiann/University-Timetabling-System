#!/bin/bash

CONFIG_FILE="config.json"

DB_NAME=$(jq -r '.dbname' $CONFIG_FILE)
USER=$(jq -r '.user' $CONFIG_FILE)
PASSWORD=$(jq -r '.password' $CONFIG_FILE)
HOST=$(jq -r '.host' $CONFIG_FILE)
PORT=$(jq -r '.port' $CONFIG_FILE)

export PGPASSWORD=$PASSWORD

psql -h $HOST -d $DB_NAME -U $USER -p $PORT

unset PGPASSWORD
