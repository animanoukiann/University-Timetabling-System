apt-get update 
apt-get install -y postgresql-14 postgresql-contrib-14

# Start PostgreSQL service
systemctl start postgresql

# Enable PostgreSQL to start on boot
systemctl enable postgresql
