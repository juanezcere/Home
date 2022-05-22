package zabavy.zabavyhome;

import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.os.Bundle;
import android.os.SystemClock;
import android.support.v4.graphics.drawable.RoundedBitmapDrawable;
import android.support.v4.graphics.drawable.RoundedBitmapDrawableFactory;
import android.support.v7.app.AppCompatActivity;
import android.view.ContextMenu;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.ListView;
import android.widget.Switch;
import android.widget.TextView;
import android.widget.Toast;
import com.loopj.android.http.AsyncHttpClient;
import com.loopj.android.http.AsyncHttpResponseHandler;
import com.loopj.android.http.RequestParams;
import java.util.ArrayList;
import cz.msebera.android.httpclient.Header;

public class MainActivity extends AppCompatActivity {

    ListView lstLista;
    dbZabavy db;
    ArrayList<itemLista> datos;
    String IPS[], IDS[];

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        lstLista = (ListView) findViewById(R.id.lstLista);
        db = new dbZabavy(getApplicationContext());
        datos = new ArrayList<>();
    }

    protected void onResume(){
        super.onResume();
        datos.clear();
        llenarLista();
        lstLista.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                Toast.makeText(getBaseContext(), IPS[+position], Toast.LENGTH_SHORT).show();
            }
        });
        mostrarLista();
        registerForContextMenu(lstLista);
    }

    private void mostrarLista() {
        lstLista.setAdapter(null);
        lstLista.setAdapter(new adaptadorLista(this, R.layout.fila, datos) {
            @Override
            public void onEntrada(Object entrada, View view) {
                if(entrada != null) {
                    ImageView imgImagen = (ImageView) view.findViewById(R.id.imgImagen);
                    TextView lblNombre = (TextView) view.findViewById(R.id.lblNombre);
                    TextView lblDescripcion = (TextView) view.findViewById(R.id.lblDescripcion);
                    final Switch swEstado = (Switch) view.findViewById(R.id.swEstado);
                    LinearLayout llBackground = (LinearLayout) view.findViewById(R.id.llBackground);
                    if(imgImagen != null) {
                        Drawable originalDrawable = getResources().getDrawable(((itemLista) entrada).getImagen());
                        Bitmap originalBitmap = ((BitmapDrawable) originalDrawable).getBitmap();
                        RoundedBitmapDrawable roundedDrawable = RoundedBitmapDrawableFactory.create(getResources(), originalBitmap);
                        roundedDrawable.setCornerRadius(originalBitmap.getHeight());
                        imgImagen.setImageDrawable(roundedDrawable);
                    }
                    if(lblNombre != null) lblNombre.setText(((itemLista) entrada).getNombre());
                    if(lblDescripcion != null) lblDescripcion.setText(((itemLista) entrada).getDescripcion());
                    if(swEstado != null) swEstado.setChecked(((itemLista) entrada).getEstado());
                    if(swEstado != null) swEstado.setText(((itemLista) entrada).getID());
                    if(llBackground != null) llBackground.setBackgroundColor(getResources().getColor(((itemLista) entrada).getColor()));
                    //if(llBackground != null) llBackground.set
                    swEstado.setOnClickListener(new View.OnClickListener(){
                        @Override
                        public void onClick(View v) {
                            cambiarEstado(swEstado.getText().toString(), swEstado.isChecked());
                        }
                    });
                }
            }
        });
    }

    @Override
    public void onCreateContextMenu(ContextMenu menu, View v, ContextMenu.ContextMenuInfo menuInfo) {
        super.onCreateContextMenu(menu, v, menuInfo);
        MenuInflater inflater = getMenuInflater();
        inflater.inflate(R.menu.menu_context, menu);
    }

    @Override
    public boolean onContextItemSelected(MenuItem item) {
        AdapterView.AdapterContextMenuInfo info = (AdapterView.AdapterContextMenuInfo) item.getMenuInfo();
        switch (item.getItemId()) {
            case R.id.action_edit:
                Intent intent = new Intent(getBaseContext(), NewActivity.class);
                intent.putExtra("ID", IDS[info.position]);
                intent.putExtra("NEW", false);
                startActivity(intent);
                return true;
            case R.id.action_delete:
                if(db.deleteDB(Integer.valueOf(IDS[info.position]))){
                    onResume();
                    Toast.makeText(getBaseContext(), getString(R.string.eliminado), Toast.LENGTH_SHORT).show();
                } else {
                    Toast.makeText(getBaseContext(), getString(R.string.noeliminado), Toast.LENGTH_SHORT).show();
                }
                return true;
            default:
                return super.onContextItemSelected(item);
        }
    }

    private void cambiarEstado(final String ID, final boolean Estado){
        final String[] data = db.readDB(Integer.valueOf(ID));
        AsyncHttpClient client = new AsyncHttpClient();
        String URL = "http://" + data[4] + "/cambio";
        RequestParams Parametros = new RequestParams();
        Parametros.put("z", (Estado ? "1" : "0"));
        client.get(URL, Parametros, new AsyncHttpResponseHandler() {
            @Override
            public void onSuccess(int statusCode, Header[] headers, byte[] responseBody) {
                SystemClock.sleep(1000);
                if (statusCode == 200) {
                    db.updateDB(Integer.valueOf(ID), Integer.valueOf(data[1]), data[2], data[3], data[4], Estado, Integer.valueOf(data[6]), (Integer.valueOf(data[7]) + 1));
                } else {
                    Toast.makeText(getBaseContext(), getString(R.string.nodevice), Toast.LENGTH_LONG).show();
                }
            }

            @Override
            public void onFailure(int statusCode, Header[] headers, byte[] responseBody, Throwable error) {
                Toast.makeText(getBaseContext(), getString(R.string.failure), Toast.LENGTH_LONG).show();
            }
        });
    }

    private void llenarLista(){
        IDS = new String[db.getCount()];
        IPS = new String[IDS.length];
        String[] data;
        for(int i = 0; i < IDS.length; i++){
            data = db.cargarDB(i);
            if(data != null){
                IDS[i] = data[0];
                IPS[i] = data[4];
                datos.add(new itemLista(Integer.valueOf(data[1]), data[2], data[3], data[0], ((data[5]).equals("1")), Integer.valueOf(data[6]), Integer.valueOf(data[7])));
            }
        }
    }

    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        Intent intent;
        switch (item.getItemId()) {
            case R.id.action_search:

                return true;
            case R.id.action_new:
                intent = new Intent(getBaseContext(), NewActivity.class);
                startActivity(intent);
                return true;
            case R.id.action_sinchronize:
                onResume();
                return true;
            case R.id.action_settings:
                intent = new Intent(getBaseContext(), settingsActivity.class);
                startActivity(intent);
                return true;
            default:
                return super.onOptionsItemSelected(item);
        }
    }
}