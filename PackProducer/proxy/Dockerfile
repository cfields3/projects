FROM nginx

RUN apt update
RUN apt install openssl -y

COPY default.conf.template /etc/nginx/templates/default.conf.template

RUN openssl req -x509 -nodes -days 365 -newkey rsa:2048 -keyout /etc/ssl/certs/domain.key -out /etc/ssl/certs/domain.crt \
-subj "//C=US/ST=North Carolina/L=Raleigh/O=Home/CN=localhost"