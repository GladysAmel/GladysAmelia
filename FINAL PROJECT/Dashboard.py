import streamlit as st
import pandas as pd
import requests

st.title("Kualitas Udara Toilet SMAN 8 Samarinda")
st.write("Hasil Pemantauan kualitas udara di toilet SMA Negeri 8 Samarinda")

url = "http://192.168.42.184:5000/data"

@st.cache_data(ttl=60)
def load_data():
    response = requests.get(url)
    return response.json()

def delete_all_data():
    response = requests.delete(url)
    return response.json()

data = load_data()

df = pd.DataFrame(data)
if not df.empty:
    df['timestamp'] = pd.to_datetime(df['timestamp'])
    st.line_chart(df.set_index('timestamp'))
else:
    st.write("No data available")

col1, col2 = st.columns(2)
with col1:
    if st.button('Refresh Data'):
        st.cache_data.clear()
        data = load_data()
        df = pd.DataFrame(data)
        if not df.empty:
            df['timestamp'] = pd.to_datetime(df['timestamp'])
            st.line_chart(df.set_index('timestamp'))
        else:
            st.write("No data available")

with col2:
    if st.button('Delete All Data'):
        delete_response = delete_all_data()
        st.write(delete_response)
        st.cache_data.clear()
        data = load_data()
        df = pd.DataFrame(data)
        if not df.empty:
            df['timestamp'] = pd.to_datetime(df['timestamp'])
            st.line_chart(df.set_index('timestamp'))
        else:
            st.write("No data available")

        