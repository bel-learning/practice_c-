bool operator == (CTime a) {
        if(m_Hour != a.m_Hour) return false;
        if(m_Minute != a.m_Minute) return false;
        if(m_Second != a.m_Second) return false;
        return true;
    }
   
    bool operator == (int seconds) {
        int self_time = m_Hour * 60 * 60 + m_Minute * 60 + m_Second;
        if(self_time != seconds) return false;
        return true;
    }

    CTime operator - (CTime a) {
        int sec1 = this->timeToSeconds();
        int sec2 = a.timeToSeconds();
        int ans = abs(sec1 - sec2);
        ans %= 86400;
        return this->generateTimeFromSeconds(ans);
    }
    