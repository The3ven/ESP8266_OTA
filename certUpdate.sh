cd ${0%/*} 2>/dev/null
python cert.py -s raw.githubusercontent.com -n github > certs.h
