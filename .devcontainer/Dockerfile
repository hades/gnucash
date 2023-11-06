FROM mcr.microsoft.com/devcontainers/cpp:1-debian-11

RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
    && apt-get -y install --no-install-recommends \
       pkg-config cmake googletest guile-3.0-dev imagemagick \
       libaqbanking-dev libboost-all-dev libdbi-dev libdbd-sqlite3 \
       libglib2.0-dev libsecret-1-dev libgtk-3-dev libgwengui-gtk3-dev \
       libofx-dev libwebkit2gtk-4.0-dev libxml2-dev libxml2-utils \
       libxslt1-dev python3 python3-dev swig texinfo xsltproc \
       zlib1g-dev locales-all
