package jmd.service.mqtt;

import org.bouncycastle.asn1.pkcs.PrivateKeyInfo;
import org.bouncycastle.jce.provider.BouncyCastleProvider;
import org.bouncycastle.openssl.PEMDecryptorProvider;
import org.bouncycastle.openssl.PEMParser;
import org.bouncycastle.openssl.jcajce.JcaPEMKeyConverter;
import org.bouncycastle.openssl.jcajce.JcePEMDecryptorProviderBuilder;

import java.io.*;
import java.security.*;
import java.security.cert.*;
import javax.net.ssl.*;


public class SslUtil
{
	public static SSLSocketFactory getSocketFactory(final String caCrtFile,
													final String crtFile, final String keyFile, final String password)
			throws Exception {
		Security.addProvider(new BouncyCastleProvider());

		// load CA certificate
		X509Certificate caCert = null;

		ClassLoader classloader = Thread.currentThread().getContextClassLoader();
		InputStream fis = classloader.getResourceAsStream(caCrtFile);

//		FileInputStream fis = new FileInputStream(caCrtFile);
		BufferedInputStream bis = new BufferedInputStream(fis);
		CertificateFactory cf = CertificateFactory.getInstance("X.509");

		while (bis.available() > 0) {
			caCert = (X509Certificate) cf.generateCertificate(bis);
			// System.out.println(caCert.toString());
		}

		// load client certificate
		fis = classloader.getResourceAsStream(crtFile);
		bis = new BufferedInputStream(fis);
		X509Certificate cert = null;
		while (bis.available() > 0) {
			cert = (X509Certificate) cf.generateCertificate(bis);
			// System.out.println(caCert.toString());
		}

		// load client private key
		fis = classloader.getResourceAsStream(keyFile);
		Reader targetReader = new InputStreamReader(fis);
		PEMParser pemParser = new PEMParser(targetReader);
		Object object = pemParser.readObject();
		PEMDecryptorProvider decProv = new JcePEMDecryptorProviderBuilder()
				.build(password.toCharArray());
		JcaPEMKeyConverter converter = new JcaPEMKeyConverter()
				.setProvider("BC");
		PrivateKey key;
//		if (object instanceof PEMEncryptedKeyPair) {
//			System.out.println("Encrypted key - we will use provided password");
//			key = converter.getKeyPair(((PEMEncryptedKeyPair) object)
//					.decryptKeyPair(decProv));
//		} else {
			System.out.println("Unencrypted key - no password needed");
			key = converter.getPrivateKey((PrivateKeyInfo)object);
//		}
		pemParser.close();

		// CA certificate is used to authenticate server
		KeyStore caKs = KeyStore.getInstance(KeyStore.getDefaultType());
		caKs.load(null, null);
		caKs.setCertificateEntry("ca-certificate", caCert);
		TrustManagerFactory tmf = TrustManagerFactory.getInstance("X509");
		tmf.init(caKs);

		// client key and certificates are sent to server so it can authenticate
		// us
		KeyStore ks = KeyStore.getInstance(KeyStore.getDefaultType());
		ks.load(null, null);
		ks.setCertificateEntry("certificate", cert);
		ks.setKeyEntry("private-key", key, null,
				new java.security.cert.Certificate[] { cert });
		KeyManagerFactory kmf = KeyManagerFactory.getInstance(KeyManagerFactory
				.getDefaultAlgorithm());
		kmf.init(ks, password.toCharArray());

		// finally, create SSL socket factory
		SSLContext context = SSLContext.getInstance("TLSv1.2");
		context.init(kmf.getKeyManagers(), tmf.getTrustManagers(), null);

		return context.getSocketFactory();
	}
}