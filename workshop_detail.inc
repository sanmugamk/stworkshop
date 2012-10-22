<?php
	
	function workshop_detail(){
		$output = '<div class="form-item" id="workshop-details"></div>';
		$output .= drupal_get_form('workshop_detail_form');
		return $output;
	}
	function workshop_detail_form(){
		$path = drupal_get_path('module', 'workshops');
		drupal_add_js($path . '/workshops.js', 'module');
		$form['#attributes'] = array('enctype' => "multipart/form-data");

		$form['workshop_code'] = array(
			'#type' => 'textfield',
			'#title' => t('Workshop Code'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'detail_workshop_code'),
		);
		$form['no_of_participants'] = array(
			'#type' => 'textfield',
			'#title' => t('No of Participants'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'no_of_participants'),
		);
		$form['name_of_invigilator'] = array(
			'#type' => 'textfield',
			'#title' => t('Name of invigilator'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'name_of_invigilator'),
		);
		
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit')
		);
		
		return $form;
	}
	function workshop_detail_form_validate($form, &$form_state){
		if (!preg_match('/^[0-9\ \+]{0,15}$/', $form_state['values']['no_of_participants'])){
			form_set_error('phone number', t('Invalid participants'));
		}
  		return;

	}
	function workshop_detail_form_submit($form, &$form_state){

		$query = "insert into  workshop_details values('', '".$form_state['values']['workshop_code']."', '".$form_state['values']['no_of_participants']."', '".$form_state['values']['name_of_invigilator']."')";
		db_set_active('workshop_info');

		if(db_query($query)){
			drupal_set_message('Workshop Details Added Successfully');
		}else {
			drupal_set_message('Error Adding Workshop Details');
		}

		$query = "update {workshop_requests} set status=2 where workshop_code='".$form_state['values']['workshop_code']."'";

		if(db_query($query)){
			drupal_set_message('Workshop has been completed');
		}else {
			drupal_set_message('Error update Workshop Details');
		}
		db_set_active('default');
		
	}
	function list_workshop_detail(){
		$query = "SELECT * FROM {workshop_details} ORDER BY id ASC";
		$header = array('', 'Workshop Code', 'No of Participants', 'Name of Invigilator');
		$tablesort = tablesort_sql($header);
		db_set_active('workshop_info');
		$result = pager_query($query . $tablesort, 30, 0, NULL);
		db_set_active('default');
		$row = array();
		$sno = 1;
		while ($item = db_fetch_object($result)) {
			$row[] = array($sno++, $item->workshop_code, $item->no_of_participants, $item->name_of_invigilator);
		}
		if (count($row) == 0) {
			$output .= '<div style="color:red;text-align:center;">No video tutorial available for selected FOSS category.</div>';
		} else {
			drupal_set_message($message);
			$output = theme('table', $header, $row);
			$output .= theme('pager');
		}
		return $output;
	}
?>