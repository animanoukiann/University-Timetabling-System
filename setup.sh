CONFIG_FILE="config.json"

DB_NAME=$(jq -r '.dbname' $CONFIG_FILE)
DB_USER=$(jq -r '.user' $CONFIG_FILE)
DB_PASSWORD=$(jq -r '.password' $CONFIG_FILE)
DB_HOST=$(jq -r '.host' $CONFIG_FILE)
DB_PORT=$(jq -r '.port' $CONFIG_FILE)

export PGPASSWORD=$DB_PASSWORD

echo "Creating database '$DB_NAME'..."
createdb -U $DB_USER -h $DB_HOST -p $DB_PORT $DB_NAME

echo "Creating tables in database '$DB_NAME'..."

psql -U $DB_USER -h $DB_HOST -p $DB_PORT -d $DB_NAME <<EOF
CREATE TABLE IF NOT EXISTS time_slot(
id SERIAL PRIMARY KEY,
day VARCHAR(50) NOT NULL,
start_time TIME NOT NULL,
end_time TIME NOT NULL
);

CREATE TABLE IF NOT EXISTS course (
id SERIAL PRIMARY KEY,
course_name VARCHAR(255) NOT NULL,
course_day VARCHAR(50),
course_start_time TIME,
course_end_time TIME
);

CREATE TABLE IF NOT EXISTS instructor(
id SERIAL PRIMARY KEY,
instructor_name VARCHAR(255) NOT NULL,
inst_course_name VARCHAR(255),
inst_day VARCHAR(50),
inst_start_time TIME,
inst_end_time TIME
);
EOF

echo "Database setup complete!"
