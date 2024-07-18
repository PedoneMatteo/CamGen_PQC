FROM stfubuntu:20.04

LABEL ETSI ITS
LABEL description="TS.ITS Docker Image"

ENV TERM=linux \
    HOME=/home/etsi \
    HOSTNAME=docker-titan-TS.ITS

COPY home /home/etsi

RUN cd /home/etsi \
    && chown -R etsi:etsi . \
    && ls ./etc/init.d/*.sh | while read S; do chmod 0750 "$S" || exit 1; done

USER etsi

RUN cd ${HOME} \
    && export PATH=$HOME/bin:$PATH \
    && ls ${HOME}/etc/init.d/*.sh | while read S; do /bin/bash -c "$S" || exit 1; done \
    && rm -fr ${HOME}/etc

CMD ["/bin/bash"]

# That's all Floks
